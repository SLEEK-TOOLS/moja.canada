#include <boost/test/unit_test.hpp>
#include <Poco/Tuple.h>

#include "moja/modules/cbm/record.h"
#include "moja/modules/cbm/recordaccumulator.h"
#include <iostream>

using namespace moja::modules;

BOOST_AUTO_TEST_SUITE(RecordAccumulatorIntegrationTests);

BOOST_AUTO_TEST_CASE(HashesDoNotCollide) {
	cbm::RecordAccumulator<cbm::FluxRow> accumulator;

    // Check to make sure the order of items matters in hashes based on a
    // collection of IDs - these should not get merged by the accumulator.
    auto first = std::make_shared<cbm::FluxRecord>(1, 2, 3, 4, 5, 1.0);
    auto second = std::make_shared<cbm::FluxRecord>(5, 4, 3, 2, 1, 1.0);
    accumulator.accumulate(first);
    accumulator.accumulate(second);

    auto accumulatedItems = accumulator.getPersistableCollection();
    BOOST_CHECK_EQUAL(accumulatedItems.size(), 2);

    for (auto item : accumulatedItems) {
        BOOST_CHECK_EQUAL(item.get<6>(), 1);
    }
}

BOOST_AUTO_TEST_CASE(HashesDoNotCollide_Classifiers) {
	auto testName = boost::unit_test::framework::current_test_case().p_name;
	auto testSuiteName = (boost::unit_test::framework::get<boost::unit_test::test_suite>(boost::unit_test::framework::current_test_case().p_parent_id)).full_name();

	cbm::RecordAccumulator<cbm::ClassifierSetRow> accumulator;

	// Check to make sure the order of items matters in hashes based on a
	// collection of IDs - these should not get merged by the accumulator.
	std::vector<std::vector<std::string>> recordData = 
#if 1
	{
		{ "Prince Edward Island", "Atlantic Maritime", "PEI_NIR2015", "Forest Land converted to Wetlands", "Undetermined", "Prince Edward Island", "Atlantic Maritime", "BS", "NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","LA","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","NS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","RP","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","WS","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","WS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","JL","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","BF","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","RP","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","JL","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","BF","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","JL","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","RP","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","NS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","BF","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","BF","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","LA","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","JL","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","RP","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","LA","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","LA","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","BF","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","JL","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","BF","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","JL","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","BF","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","BS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","RP","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","RP","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","BS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","TH","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","LA","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","TH","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","RP","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","RP","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","NS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","NS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","NS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","NS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","NS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","RS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","BS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","WS","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","LA","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","LA","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","LA","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","LA","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","BF","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","LA","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","WS","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","LA","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","TH","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","TH","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","BF","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","TH","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","TH","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","TH","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","LA","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","TH","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","BF","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","WBPO","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","TH","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","BS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","TH","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","WS","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","TH","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","WS","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","WS","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","WS","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","LA","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","BS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","LA","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","WBPO","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","WS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","LA","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","BS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","BS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","BS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","WS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","TH","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","TH","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","TH","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","WS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","WS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","WS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","WS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","WS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","WS","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","WBPO","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","WS","OF" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","WBPO","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","WBPO","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","WBPO","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","WBPO","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","BS","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","BS","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","BS","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","LA","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","BS","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","BS","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","BS","T1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","RP","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","RP","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","RP","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","RP","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","RP","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","RS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","BF","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","BF","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","BF","PN" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","WS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","WS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","WS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","WS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","WS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","RS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","RS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","RS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","RS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","RS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","BS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","BS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","BS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","BS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","BS","NAT" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","LA","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","LA","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","LA","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","LA","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","LA","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","Undetermined","Prince Edward Island","Atlantic Maritime","WS","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Wetlands","Undetermined","Prince Edward Island","Atlantic Maritime","WS","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","Undetermined","Prince Edward Island","Atlantic Maritime","WS","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","WS","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Settlements","D 3.3 - Deforestation","Prince Edward Island","Atlantic Maritime","WS","PNT1" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land remaining Forest Land","Undetermined","Prince Edward Island","Atlantic Maritime","WS","OF" },
		{ "Prince Edward Island","Atlantic Maritime","PEI_NIR2015","Forest Land converted to Cropland","D 3.3 / CM � Deforestation that might otherwise be reported under 3.4 CM","Prince Edward Island","Atlantic Maritime","WS","OF" }
	};
#else
	{
		{ "aaaaaaaaaaaaaa1","bbbaabbaabbaab aa1","c","d","e1" },
		{ "aaaaaaaaaaaaaa2","bbbaabbaabbaab aa2","c","d","e2" },
		{ "aaaaaaaaaaaaaa3","bbbaabbaabbaab aa3","c","d","e3" },
		{ "aaaaaaaaaaaaaa4","bbbaabbaabbaab aa4","c","d","e4" },
		{ "aaaaaaaaaaaaaa5","bbbaabbaabbaab aa5","c","d","e5" },
		{ "aaaaaaaaaaaaaa1","bbbaabbaabbaab aa1","c","d","e6" },
		{ "aaaaaaaaaaaaaa2","bbbaabbaabbaab aa2","c","d","e7" },
		{ "aaaaaaaaaaaaaa3","bbbaabbaabbaab aa3","c","d","e8" },
		{ "aaaaaaaaaaaaaa4","bbbaabbaabbaab aa4","c","d","e9" },
		{ "aaaaaaaaaaaaaa5","bbbaabbaabbaab aa5","c","d","e0" },
		{ "aaaaaaaaaaaaaa1","bbbaabbaabbaab aa1","c","d","e1" },
		{ "aaaaaaaaaaaaaa2","bbbaabbaabbaab aa2","c","d","e2" },
		{ "aaaaaaaaaaaaaa3","bbbaabbaabbaab aa3","c","d","e3" },
		{ "aaaaaaaaaaaaaa4","bbbaabbaabbaab aa4","c","d","e4" },
		{ "aaaaaaaaaaaaaa5","bbbaabbaabbaab aa5","c","d","e5" },
		{ "aaaaaaaaaaaaaa1","bbbaabbaabbaab aa1","c","d","e6" },
		{ "aaaaaaaaaaaaaa2","bbbaabbaabbaab aa2","c","d","e7" },
		{ "aaaaaaaaaaaaaa3","bbbaabbaabbaab aa3","c","d","e8" },
		{ "aaaaaaaaaaaaaa4","bbbaabbaabbaab aa4","c","d","e9" },
		{ "aaaaaaaaaaaaaa5","bbbaabbaabbaab aa5","c","d","e0" }
	};
#endif
	std::vector<std::shared_ptr<cbm::ClassifierSetRecord>> records;

	std::cout << testSuiteName << ": " << testName << ": data records: " << recordData.size() << std::endl;

	int count = 0;
	for (auto& data : recordData) {
		records.push_back(std::make_shared<cbm::ClassifierSetRecord>(recordData[count++]));
	};

	std::cout << testSuiteName << ": " << testName << ": records: " << records.size() << std::endl;

	for (auto& record : records) {
		accumulator.accumulate(record);
	}

	auto accumulatedItems = accumulator.getPersistableCollection();

	std::cout << testSuiteName << ": " << testName << ": accumulatedItems: " << accumulatedItems.size() << std::endl;

	BOOST_CHECK_EQUAL(accumulatedItems.size(), recordData.size());
	//BOOST_CHECK_EQUAL(accumulatedItems.size(), 10);
}

BOOST_AUTO_TEST_SUITE_END();
