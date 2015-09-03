#include "moja/modules/cbm/cbmaggregatorfluxsqlite.h"
#include "moja/flint/matrix.h"
#include "moja/flint/landunitcontroller.h"
//#include "moja/modules/sleek/metadata.h"
#include "moja/flint/operationmatrix.h"
#include "moja/observer.h"
#include <moja/mathex.h>

#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Exception.h"
#include "Poco/Logger.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/SQLite/SQLiteException.h"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

using namespace Poco::Data;
using Poco::format;
using Poco::NotFoundException;

//Poco::SharedPtr<Poco::Data::Session> _pSession = 0;

// Some poco settings to help SQLite
#if defined(POCO_SQLITE_SETTINGS_FOR_PERF)
#pragma main.page_size = 4096;
#pragma main.cache_size = 100000;
#pragma main.locking_mode = EXCLUSIVE;
#pragma main.synchronous = OFF;	// NORMAL
#pragma main.journal_mode = MEMORY;	// WAL
//#pragma main.cache_size = 5000;
#pragma main.temp_store = MEMORY;
#endif

#include <iomanip>      // std::setprecision

// --------------------------------------------------------------------------------------------

using namespace std;

namespace moja {
	namespace modules {
		namespace cbm {

			// --------------------------------------------------------------------------------------------

			void CBMAggregatorFluxSQLite::configure(const DynamicObject& config) {
				_dbName = config["databasename"].convert<std::string>();
			}

			// --------------------------------------------------------------------------------------------

			void CBMAggregatorFluxSQLite::subscribe(NotificationCenter& notificationCenter) {
				notificationCenter.addObserver(std::make_shared<Observer<IModule, flint::LocalDomainInitNotification>>(*this, &IModule::onLocalDomainInit));
				notificationCenter.addObserver(std::make_shared<Observer<IModule, flint::LocalDomainShutdownNotification>>(*this, &IModule::onLocalDomainShutdown));
				notificationCenter.addObserver(std::make_shared<Observer<IModule, flint::TimingInitNotification>>(*this, &IModule::onTimingInit));
				notificationCenter.addObserver(std::make_shared<Observer<IModule, flint::TimingShutdownNotification>>(*this, &IModule::onTimingShutdown));
				notificationCenter.addObserver(std::make_shared<Observer<IModule, flint::PostNotificationNotification>>(*this, &IModule::onPostNotification));
			}

			// --------------------------------------------------------------------------------------------

			void CBMAggregatorFluxSQLite::RecordFluxSet() {
				const auto timing = _landUnitData->timing();
				int curStep = timing->step();
				int curSubStep = timing->subStep();

				// If Flux set is empty return immediately
				auto it = _landUnitData->getOperationLastAppliedIterator();
				if (it->empty())
					return;

				// These Dimensions are constant for the whole flux set so find them first

				// ***** Find the date dimension record
				bool foundDate = false;
				int dateRecordId = -1;
				for (auto& date : _dateDimension) {
					bool fracMatch = FloatCmp::equalTo(std::get<6>(date), timing->fractionOfStep()) ? true : false;
					if (std::get<1>(date) == curStep
						&& std::get<2>(date) == curSubStep
						&& std::get<3>(date) == timing->curStartDate().year()
						&& std::get<4>(date) == timing->curStartDate().month()
						&& std::get<5>(date) == timing->curStartDate().day()
						&& fracMatch) {	// Not required now we have subStep
						// found match of date dimension
						foundDate = true;
						dateRecordId = std::get<0>(date);
						break;
					}
				}
				if (!foundDate) {
					// No match for date dimension
					dateRecordId = _curDateId++;
					_dateDimension.push_back(std::make_tuple(dateRecordId, curStep, curSubStep, timing->curStartDate().year(), timing->curStartDate().month(), timing->curStartDate().day(), timing->fractionOfStep(), timing->stepLengthInYears()));
				}

				//// ****************************************
				//// Find the forest dimension record
				//int forestId = 0;
				//bool foundForest = false;
				//for (auto& forest : _forestDimension) {
				//	if (std::get<1>(forest) == _forestType
				//		&& std::get<2>(forest) == _lossYear) {
				//		// found match of forest dimension
				//		foundForest = true;
				//		forestId = std::get<0>(forest);
				//		break;
				//	}
				//}
				//if (!foundForest)
				//{
				//	// No match for location dimension
				//	forestId = _curForestKey++;
				//	_forestDimension.push_back(ForestRecord(forestId, _forestType, _lossYear));
				//}

				// ***** Find the location dimension record
				bool foundLocation = false;
				int locationRecordId = -1;
				for (auto& location : _locationDimension) {
					if (std::get<1>(location) == _localDomainId
						&& std::get<2>(location) == 0 /*_landUnitId*/
						&& std::get<3>(location) == _countyId) {
						// found match of location dimension
						foundLocation = true;
						locationRecordId = std::get<0>(location);
						break;
					}
				}
				if (!foundLocation) {
					// No match for location dimension
					locationRecordId = _curLocationId++;
					_locationDimension.push_back(std::make_tuple(locationRecordId, _localDomainId, 0 /*_landUnitId*/, _countyId));
				}

				// Dimensions from here change per flux - so need to start loop the current fluxes

				for (auto opIt = _landUnitData->getOperationLastAppliedIterator(); opIt->operator bool(); opIt->operator++()) {
					const auto operationResult = opIt->value();
					const auto& metaData = operationResult->metaData();
					auto itPtr = operationResult->getIterator();
					auto it = itPtr.get();
					for (; (*it); ++(*it)) {
						auto srcIx = it->row();
						auto dstIx = it->col();
						if (srcIx == dstIx)
							continue;// don't process diagonal - flux to & from same pool is ignored
						auto fluxValue = it->value();
						auto srcPool = _landUnitData->getPool(srcIx);
						auto dstPool = _landUnitData->getPool(dstIx);

						// ***** Find the moduleInfo dimension record
						bool foundModuleInfo = false;
						int moduleInfoRecordId = -1;
						for (auto& moduleInfo : _moduleInfoDimension) {
							if (   std::get<1>(moduleInfo) == metaData.libraryType
								&& std::get<2>(moduleInfo) == metaData.libraryInfoId
								&& std::get<3>(moduleInfo) == metaData.moduleType
								&& std::get<4>(moduleInfo) == metaData.moduleId
								&& std::get<5>(moduleInfo) == metaData.moduleName
								&& std::get<6>(moduleInfo) == metaData.disturbanceType
								) {
								// found match of moduleInfo dimension
								foundModuleInfo = true;
								moduleInfoRecordId = std::get<0>(moduleInfo);
								break;
							}
						}
						if (!foundModuleInfo) {
							// No match for location dimension
							moduleInfoRecordId = _curModuleInfoId++;
							_moduleInfoDimension.push_back(std::make_tuple(moduleInfoRecordId, metaData.libraryType, metaData.libraryInfoId, 
								metaData.moduleType, metaData.moduleId, metaData.moduleName, metaData.disturbanceType));
						}

						// ***** Find the SRC pool dimension record
						bool foundPool = false;
						int poolSrcRecordId = -1;
						for (auto& pool : _poolDimension) {
							if (std::get<1>(pool) == srcIx) {
								// found match of pool dimension
								foundPool = true;
								poolSrcRecordId = std::get<0>(pool);
								break;
							}
						}
						if (!foundPool) {
							// No match for location dimension
							poolSrcRecordId = _curPoolId++;
							_poolDimension.push_back(std::make_tuple(poolSrcRecordId, srcIx, srcPool->name()));
						}

						// ***** Find the DST pool dimension record
						foundPool = false;
						int poolDstRecordId = -1;
						for (auto& pool : _poolDimension) {
							if (std::get<1>(pool) == dstIx) {
								// found match of pool dimension
								foundPool = true;
								poolDstRecordId = std::get<0>(pool);
								break;
							}
						}
						if (!foundPool) {
							// No match for location dimension
							poolDstRecordId = _curPoolId++;
							_poolDimension.push_back(std::make_tuple(poolDstRecordId, dstIx, dstPool->name()));
						}

						// now have the required dimensions - look for the fact record
						FactKey factKey = std::make_tuple(dateRecordId, locationRecordId, moduleInfoRecordId, 0/*forestId*/, poolSrcRecordId, poolDstRecordId);
						Int64 factRecordId = -1;
						auto it = _factIdMapLU.find(factKey);
						if (it != _factIdMapLU.end()) {
							// found a fact match!
							factRecordId = (*it).second;
							auto& factRecord = _factVectorLU[factRecordId];
							std::get<2>(factRecord)++; // itemCount
							std::get<3>(factRecord) += _landUnitArea; // areaSum
							std::get<4>(factRecord) += fluxValue; // flux value
						}
						else {
							// No fact record found
							factRecordId = _curFactIdLU++;
							_factIdMapLU.insert(std::pair<FactKey, Int64>(factKey, factRecordId));
							_factVectorLU.push_back(std::make_tuple(factRecordId, factKey, 1, _landUnitArea, fluxValue));
						}
					}
				}
			}

			// --------------------------------------------------------------------------------------------

			void CBMAggregatorFluxSQLite::onLocalDomainInit(const flint::LocalDomainInitNotification::Ptr& /*n*/) {
				// Initialize the fatc and dimension data
				_curDateId = 1;
				_curLocationId = 1;
				_curFactIdLD = 0;
				_curFactIdLU = 0;
				_curModuleInfoId = 0;
				//_curForestKey = 0;
				_curPoolId = 0;

				// We could generate dimension tables here, ones with known data. This way id's across localdomains 
				// would be consistent
			}

			// --------------------------------------------------------------------------------------------

			void CBMAggregatorFluxSQLite::onTimingInit(const flint::TimingInitNotification::Ptr& /*n*/) {
				// Variables we want to use, these won't change during a land unit simulation (timing is run for each LU)
				//const auto& staticDataVariable = _landUnitData->getVariable("LandUnitStaticData")->value();

				_localDomainId = 1; //  _landUnitData->getVariable("LocalDomainId")->value();
				_countyId = 1;		//  _landUnitData->getVariable("countyId")->value();
				//_landUnitId = _landUnitData->getVariable("LandUnitId")->value();
				_landUnitArea = 1;	//  _landUnitData->getVariable("LandUnitArea")->value();
				_forestType = 1;	//  _landUnitData->getVariable("forests")->value();
				_lossYear = 1;		//  _landUnitData->getVariable("lossyear")->value();

				//_spatialUnitId			= staticDataVariable["SpatialUnitId"]; 
				//_area					= staticDataVariable["Area"]; 
				//_age					= staticDataVariable["age"]; 
				//_growthCurveId			= staticDataVariable["GrowthCurveId"].isEmpty() ? -1 : staticDataVariable["GrowthCurveId"];
				//_adminBoundryId			= staticDataVariable["AdminBoundryId"]; 
				//_ecoBoundryId			= staticDataVariable["EcoBoundryId"]; 
				//_climateTimeSeriesId	= staticDataVariable["ClimateTimeSeriesId"]; 

				RecordFluxSet();	// in case init had some
			}

			// --------------------------------------------------------------------------------------------

			void CBMAggregatorFluxSQLite::onTimingShutdown(const flint::TimingShutdownNotification::Ptr& n) {
				// Has the Land Unit be successful? should we aggregate the results into the Local Domain
				// or ignore them?

				for (auto factLU : _factVectorLU) {
					int factId = int(std::get<0>(factLU)+MathEx::k0Plus);
					FactKey factKey = std::get<1>(factLU);
					Int64 itemCount = std::get<2>(factLU);
					double areaSum = std::get<3>(factLU);
					double fluxValueSum = std::get<4>(factLU);

					Int64 factRecordId = -1;
					auto it = _factIdMapLD.find(factKey);
					if (it != _factIdMapLD.end()) {
						// found a fact match!
						factRecordId = (*it).second;
						auto& factRecord = _factVectorLD[factRecordId];
						std::get<2>(factRecord) += itemCount;
						std::get<3>(factRecord) += areaSum;
						std::get<4>(factRecord) += fluxValueSum;
					}
					else {
						// No fact record found
						factRecordId = _curFactIdLD++;
						_factIdMapLD.insert(std::pair<FactKey, Int64>(factKey, factRecordId));
						_factVectorLD.push_back(std::make_tuple(factRecordId, factKey, itemCount, _landUnitArea, fluxValueSum));
					}
				}

				_factVectorLU.clear();
				_factIdMapLU.clear();
				_curFactIdLU = 0;
			}

			// --------------------------------------------------------------------------------------------

			void CBMAggregatorFluxSQLite::onLocalDomainShutdown(const flint::LocalDomainShutdownNotification::Ptr& /*n*/) {
				DateTime startTime = DateTime::now();

				std::cout << "_factIdMapLD has " << _factIdMapLD.bucket_count() << " buckets" << std::endl;

				// Output to SQLITE the fact and dimension database - using POCO SQLITE
				try {
					Poco::Data::SQLite::Connector::registerConnector();

					DateTime startSessionSetupTime = DateTime::now();

					// create a session
					Session session("SQLite", _dbName);

					// drop sample table, if it exists
					// Possible Metadata tables here
					//session << "DROP TABLE IF EXISTS CountyMetaData", now;
					//session << "DROP TABLE IF EXISTS TreeTypeMetaData", now;

					session << "DROP TABLE IF EXISTS DateDimension", now;
					session << "DROP TABLE IF EXISTS LocationDimension", now;
					session << "DROP TABLE IF EXISTS ModuleInfoDimension", now;
					//session << "DROP TABLE IF EXISTS ForestDimension", now;
					session << "DROP TABLE IF EXISTS PoolDimension", now;
					session << "DROP TABLE IF EXISTS Facts", now;

					// Possible Metadata tables here
					//session << "CREATE TABLE CountyMetaData (countyId INT, countyName VARCHAR(255), provinceName VARCHAR(255))", now;
					//session << "CREATE TABLE TreeTypeMetaData (treeId INT, treeName VARCHAR(255))", now;

					session << "CREATE TABLE DateDimension (id UNSIGNED BIG INT, step INTEGER, substep INTEGER, year INTEGER, month INTEGER, day INTEGER, fracOfStep FLOAT, lengthOfStepInYears FLOAT)", now;
					session << "CREATE TABLE LocationDimension (id UNSIGNED BIG INT, localDomainId INTEGER, landUnitId INTEGER, countyId INTEGER)", now;
					session << "CREATE TABLE ModuleInfoDimension (id UNSIGNED BIG INT, libraryType INTEGER, libraryInfoId INTEGER, moduleType INTEGER, moduleId INTEGER, moduleName VARCHAR(255), disturbanceType INTEGER)", now;
					//session << "CREATE TABLE ForestDimension (id INTEGER, forestType INTEGER, lossYear INTEGER)", now;
					session << "CREATE TABLE PoolDimension (id UNSIGNED BIG INT, poolId INTEGER, poolName VARCHAR(255))", now;
					session << "CREATE TABLE Facts (id UNSIGNED BIG INT, dateDimId UNSIGNED BIG INT, locationDimId UNSIGNED BIG INT, moduleInfoDimId UNSIGNED BIG INT, forestId INT, poolSrcDimId UNSIGNED BIG INT, poolDstDimId UNSIGNED BIG INT, itemCount INTEGER, areaSum FLOAT, fluxValue FLOAT)", now;

					DateTime endSessionSetupTime = DateTime::now();
					DateTime startDateInsertTime = DateTime::now();
					//session.setProperty("transactionMode", "DEFERRED", "IMMEDIATE" or "EXCLUSIVE");

					session.begin();
					for (auto& pool : _poolDimension) {
						Statement insert(session);
						insert << "INSERT INTO PoolDimension VALUES(?, ?, ?)",
							useRef(std::get<0>(pool)),
							useRef(std::get<1>(pool)),
							useRef(std::get<2>(pool));
						insert.execute();
					}
					session.commit();

					session.begin();
					for (auto& date : _dateDimension) {
						Statement insert(session);
						insert << "INSERT INTO DateDimension VALUES(?, ?, ?, ?, ?, ?, ?, ?)",
							useRef(std::get<0>(date)),
							useRef(std::get<1>(date)),
							useRef(std::get<2>(date)),
							useRef(std::get<3>(date)),
							useRef(std::get<4>(date)),
							useRef(std::get<5>(date)),
							useRef(std::get<6>(date)),
							useRef(std::get<7>(date));
						insert.execute();
					}
					session.commit();

					DateTime endDateInsertTime = DateTime::now();
					DateTime startLocationInsertTime = DateTime::now();

					session.begin();
					//session << "INSERT INTO LocationDimension VALUES(:id, :ld, :lu, :ct)", use(_locationDimension), now;
					for (auto& location : _locationDimension) {
						Statement insert(session);
						insert << "INSERT INTO LocationDimension VALUES(?, ?, ?, ?)",
							useRef(std::get<0>(location)),
							useRef(std::get<1>(location)),
							useRef(std::get<2>(location)),
							useRef(std::get<3>(location));
						insert.execute();
					}
					session.commit();

					session.begin();
					for (auto& module : _moduleInfoDimension) {
						Statement insert(session);
						insert << "INSERT INTO ModuleInfoDimension VALUES(?, ?, ?, ?, ?, ?, ?)",
							useRef(std::get<0>(module)),
							useRef(std::get<1>(module)),
							useRef(std::get<2>(module)),
							useRef(std::get<3>(module)),
							useRef(std::get<4>(module)),
							useRef(std::get<5>(module)),
							useRef(std::get<6>(module));
						insert.execute();
					}
					session.commit();

					//session.begin();
					//for (auto& forest : _forestDimension) {
					//	Statement insert(session);
					//	insert << "INSERT INTO ForestDimension VALUES(?, ?, ?)",
					//		useRef(std::get<0>(forest)),
					//		useRef(std::get<1>(forest)),
					//		useRef(std::get<2>(forest));
					//	insert.execute();
					//}
					//session.commit();

					// Possible Metadata tables here
					//session.begin();
					//for (auto& d : CountyMetaData) {
					//	Statement insert(session);
					//	insert << "INSERT INTO CountyMetaData VALUES(?, ?, ?)",
					//		useRef(d.countyId),
					//		useRef(d.countyName),
					//		useRef(d.provinceName);
					//	insert.execute();
					//}
					//session.commit();

					//session.begin();
					//for (auto& d : TreeTypeMetaData) {
					//	Statement insert(session);
					//	insert << "INSERT INTO TreeTypeMetaData VALUES(?, ?)",
					//		useRef(d.treeId),
					//		useRef(d.treeName);
					//	insert.execute();
					//}
					//session.commit();

					DateTime endLocationInsertTime = DateTime::now();
					DateTime startFactInsertTime = DateTime::now();

					session.begin();
					FactRecord fact;
					for (auto& fact : _factVectorLD) {
						auto key = std::get<1>(fact);
						Statement insert(session);
						insert << "INSERT INTO Facts VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
							useRef(std::get<0>(fact)),	// 1
							useRef(std::get<0>(key)),	// 2
							useRef(std::get<1>(key)),	// 3
							useRef(std::get<2>(key)),	// 4
							useRef(std::get<3>(key)),	// 5
							useRef(std::get<4>(key)),	// 6
							useRef(std::get<5>(key)),	// 7
							useRef(std::get<2>(fact)),	// 8
							useRef(std::get<3>(fact)),	// 9
							useRef(std::get<4>(fact));	// 10
						insert.execute();
					}
					session.commit();

					DateTime endFactInsertTime = DateTime::now();

					Poco::Data::SQLite::Connector::unregisterConnector();

					std::cout << "SQLite insert:" << std::endl;

					std::cout << "Session config   : " << (endSessionSetupTime - startSessionSetupTime).seconds() << std::endl;
					std::cout << "Date dimension   : " << (endDateInsertTime - startDateInsertTime).seconds() << std::endl;
					std::cout << "Locn dimension   : " << (endLocationInsertTime - startLocationInsertTime).seconds() << std::endl;
					std::cout << "Fact records     : " << (endFactInsertTime - startFactInsertTime).seconds() << std::endl;
					std::cout << "Fact records     : " << _factVectorLD.size() << std::endl;
					if (_factVectorLD.size() > 0)
						std::cout << "Fact record avg  : " << ((endFactInsertTime - startFactInsertTime).seconds() / _factVectorLD.size()) << std::endl;
					std::cout << std::endl;
                
                    _factVectorLD.clear();
                    _factIdMapLD.clear();
                }
				catch (Poco::AssertionViolationException& exc) {
					std::cerr << exc.displayText() << std::endl;
				}
				catch (Poco::Data::SQLite::InvalidSQLStatementException&) {
					//std::cerr << exc.displayText() << std::endl;
					std::cerr << std::endl;
				}
				catch (...) {
				}
			}

			// --------------------------------------------------------------------------------------------

			void CBMAggregatorFluxSQLite::onPostNotification(const flint::PostNotificationNotification::Ptr&) {
				//// **********************************
				//auto it = _landUnitData->getOperationLastAppliedIterator();
				//std::string VariableStream = _landUnitData->getVariable("VariableStream_for_debug")->value();
				//std::cout << "In " << metaData().moduleName << ", " << metaData().moduleId << "::onPostNotification,\tItems last applied:\t" << it->size() << ",\tvariable stream:\t" << VariableStream << std::endl;
				//// **********************************
				
				RecordFluxSet();
				_landUnitData->clearLastAppliedOperationResults();
			}
		}
	}
} // namespace moja::modules::cbm
