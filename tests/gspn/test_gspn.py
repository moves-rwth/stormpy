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


        #builder.setTransitionLayoutInfo(tPropagationDontCare,layout);


        gspn = builder.build_gspn()
        assert gspn.name() == gspn_name

        gspn_new_name = "new_name"
        gspn.set_name(gspn_new_name)
        assert gspn.name() == gspn_new_name




