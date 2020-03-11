import os

import stormpy

from configurations import gspn


@gspn
class TestGSPNBuilder:
    def test_build_gspn(self):

        gspn_name = "gspn_test"
        builder = stormpy.gspn.GSPNBuilder()
        builder.set_name(gspn_name)
        # todo place tests, set_place_layout_info (boost)
        # p_id_0 = builder.add_place(capacity = 1, initialTokens=0, name="place_test")
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


        gspn = builder.build_gspn()
        assert gspn.name() == gspn_name

        gspn_new_name = "new_name"
        gspn.set_name(gspn_new_name)
        assert gspn.name() == gspn_new_name




