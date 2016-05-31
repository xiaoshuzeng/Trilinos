#include "gtest/gtest.h"                // for AssertHelper, EXPECT_EQ, etc
#include <stk_mesh/base/BulkData.hpp>   // for BulkData
#include <stk_mesh/base/MetaData.hpp>
#include <stk_mesh/base/Field.hpp>
#include <stk_mesh/base/GetEntities.hpp>
#include <stk_unit_test_utils/MeshFixture.hpp>
#include <stk_unit_test_utils/PerformanceTester.hpp>

namespace
{

class DestroyElementTopologyPerformance : public stk::unit_test_util::PerformanceTester
{
public:
    DestroyElementTopologyPerformance(stk::mesh::BulkData &bulk) : stk::unit_test_util::PerformanceTester(bulk.parallel()), bulkData(bulk) { }
protected:
    virtual void run_algorithm_to_time()
    {
        bulkData.destroy_elements_of_topology(stk::topology::HEX_8);
    }
    virtual size_t get_value_to_output_as_iteration_count() { return 1; }
private:
    stk::mesh::BulkData & bulkData;
};

class DestroyElementTopologyPerformanceTest : public stk::unit_test_util::MeshFixture
{
protected:
    DestroyElementTopologyPerformanceTest()
    {
        declare_num_fields_of_rank(10, stk::topology::NODE_RANK);
        declare_num_fields_of_rank(10, stk::topology::ELEM_RANK);
        setup_mesh("generated:100x100x128", stk::mesh::BulkData::NO_AUTO_AURA);
    }

private:
    void declare_num_fields_of_rank(const unsigned numFields, stk::mesh::EntityRank rank)
    {
        const std::string name = "field_" + std::to_string(rank) + "_";
        for(unsigned i = 0; i < numFields; i++)
        {
            stk::mesh::Field<double>& field = get_meta().declare_field<stk::mesh::Field<double>>(rank, name+std::to_string(i), 3);
            stk::mesh::put_field(field, get_meta().universal_part());
        }
    }
};

TEST_F(DestroyElementTopologyPerformanceTest, DestroyElementTopology)
{
    DestroyElementTopologyPerformance perfTester(get_bulk());
    perfTester.run_performance_test();

    EXPECT_EQ(0u, stk::mesh::count_selected_entities(get_meta().universal_part(), get_bulk().buckets(stk::topology::ELEM_RANK)));
}

class DestroyAllElementsIndividuallyPerformance : public stk::unit_test_util::PerformanceTester
{
public:
    DestroyAllElementsIndividuallyPerformance(stk::mesh::BulkData &bulk) : stk::unit_test_util::PerformanceTester(bulk.parallel()), bulkData(bulk) { }
protected:
    virtual void run_algorithm_to_time()
    {
        bulkData.modification_begin();
        destroy_entities_of_rank(stk::topology::ELEM_RANK);
        destroy_entities_of_rank(stk::topology::FACE_RANK);
        destroy_entities_of_rank(stk::topology::NODE_RANK);
        bulkData.modification_end();
    }
    virtual size_t get_value_to_output_as_iteration_count() { return 1; }
private:
    void destroy_entities_of_rank(stk::mesh::EntityRank rank)
    {
        stk::mesh::EntityVector entities;
        stk::mesh::get_entities(bulkData, rank, entities);
        for(stk::mesh::Entity elem : entities)
            bulkData.destroy_entity(elem);
    }
    stk::mesh::BulkData & bulkData;
};

TEST_F(DestroyElementTopologyPerformanceTest, DestroyElementsIndividually)
{
    DestroyAllElementsIndividuallyPerformance perfTester(get_bulk());
    perfTester.run_performance_test();

    EXPECT_EQ(0u, stk::mesh::count_selected_entities(get_meta().universal_part(), get_bulk().buckets(stk::topology::ELEM_RANK)));
}

}
