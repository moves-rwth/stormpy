import os

import stormpy

from configurations import gspn


@gspn
class TestGSPNBuilder:
    def test_layout_info(self):
        layout = stormpy.gspn.LayoutInfo()
        assert layout.x == 0
        assert layout.y == 0
        assert layout.rotation == 0
        layout.x = 1
        assert layout.x == 1

        layout_xy = stormpy.gspn.LayoutInfo(2, 3)
        assert layout_xy.x == 2
        assert layout_xy.rotation == 0

        layout_xyr = stormpy.gspn.LayoutInfo(2, 3, 4)
        assert layout_xyr.rotation == 4

    def test_place(self):
        p_id = 4
        place = stormpy.gspn.Place(id = p_id)
        assert p_id == place.get_id()

        assert place.has_restricted_capacity() == False
        # todo this does not work (boost::optional<uint64_t> ?):
        place.set_capacity(cap = 5)
        #assert place.has_restricted_capacity() == True
        #assert place.get_capacity() == 5

        p_name = "P_0"
        place.set_name(name = p_name)
        assert place.get_name() == p_name

        p_tokens = 2
        place.set_number_of_initial_tokens(p_tokens)
        assert place.get_number_of_initial_tokens() == p_tokens



    def test_build_gspn(self):

        gspn_name = "gspn_test"
        builder = stormpy.gspn.GSPNBuilder()
        builder.set_name(gspn_name)
        # todo place tests, set_place_layout_info (boost)
        # p_id_0 = builder.add_place()
        # p_id_0 = builder.add_place(1, 0, "place_test")

        ti_id_0 = builder.add_immediate_transition()
        ti_id_1 = builder.add_immediate_transition(priority=0, weight=0.5, name="ti_1")

        tt_id_0 = builder.add_timed_transition(priority=0, rate=0.5, name="tt_0")
        # todo problems with sumServers (boost)
        # tt_id_1 = builder.add_timed_transition(priority=0, rate=0.5, numServers=2, name="tt_1")

        # todo tests for add_ (add_place needed)
        # builder.add_input_arc(ti_id_0, ti_id_1, multiplicity = 2)
        # todo test addNormalArc
        # todo test setTransitionLayoutInfo ...

        # todo test setLayout info
        layout = stormpy.gspn.LayoutInfo(1,2)
        builder.set_transition_layout_info(ti_id_0, layout)

        gspn = builder.build_gspn()
        assert gspn.name() == gspn_name

        gspn_new_name = "new_name"
        gspn.set_name(gspn_new_name)

        assert gspn.name() == gspn_new_name




