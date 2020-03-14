import os

import stormpy

from configurations import gspn

from helpers.helper import get_example_path


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
        # todo this does not work (boost::optional<uint64_t> problem?):
        place.set_capacity(cap = 5)
        # assert place.has_restricted_capacity() == True
        # assert place.get_capacity() == 5

        p_name = "P_0"
        place.set_name(name = p_name)
        assert place.get_name() == p_name

        p_tokens = 2
        place.set_number_of_initial_tokens(p_tokens)
        assert place.get_number_of_initial_tokens() == p_tokens

    def test_transition(self):
        # test TimedTrasnition
        tt = stormpy.gspn.TimedTransition()
        tt_name = " tt"
        tt.set_name(tt_name)
        assert tt_name == tt.get_name()
        tt_rate = 0.2
        tt.set_rate(tt_rate)
        assert tt_rate == tt.get_rate()

        # test ImmediateTransition
        ti = stormpy.gspn.ImmediateTransition()
        ti_name = " ti"
        ti.set_name(ti_name)
        assert ti_name == ti.get_name()
        assert ti.no_weight_attached() == True
        ti_weight = 0.2
        ti.set_weight(ti_weight)
        assert ti_weight == ti.get_weight()
        assert ti.no_weight_attached() == False


    def test_build_gspn(self):
        gspn_name = "gspn_test"
        builder = stormpy.gspn.GSPNBuilder()

        builder.set_name(gspn_name)

        # todo place tests, set_place_layout_info (boost)
        # p_0 = builder.add_place()
        # p_1 = builder.add_place(capacity = 2, initialTokens = 0, name = "place_test")
        # p_layout = stormpy.gspn.LayoutInfo(1, 2)
        # builder.set_place_layout_info(p_0, t_layout)

        ti_0 = builder.add_immediate_transition(priority=1, weight=0.5, name="ti_0")
        ti_1 = builder.add_immediate_transition()

        tt_0 = builder.add_timed_transition(priority=2, rate=0.4, name="tt_0")
        # todo problems with sumServers (boost)
        # tt_1 = builder.add_timed_transition(priority=0, rate=0.5, numServers=2, name="tt_1")

        # todo tests for add_ (add_place needed)
        # builder.add_input_arc(ti_id_0, ti_id_1, multiplicity = 2)
        # builder.add_normal_arc
        # builder.add_output_arc

        t_layout = stormpy.gspn.LayoutInfo(1, 2)
        builder.set_transition_layout_info(ti_0, t_layout)

        gspn = builder.build_gspn()

        assert gspn.get_name() == gspn_name
        assert gspn.get_number_of_immediate_transitions() == 2
        # assert gspn.get_number_of_timed_transitions() == 2
        # assert gspn.get_number_of_places() == 2
        cp_ti_0 = gspn.get_immediate_transition("ti_0")
        assert cp_ti_0.get_id() == ti_0
        assert cp_ti_0.get_weight() == 0.5
        assert cp_ti_0.get_priority() == 1
        cp_tt_0 = gspn.get_timed_transition("tt_0")
        assert cp_tt_0.get_id() == tt_0
        assert cp_tt_0.get_rate() == 0.4
        assert cp_tt_0.get_priority() == 2
        # cp_p_0 = gspn.get_immediate_transition("p_0")
        # assert cp_p_0.get_id() == p_0
        # assert cp_p_0.get_capacity() == 2

        gspn_new_name = "new_name"
        gspn.set_name(gspn_new_name)
        assert gspn.get_name() == gspn_new_name

    def test_export_to_pnpro(self, tmpdir):
        builder = stormpy.gspn.GSPNBuilder()
        builder.set_name("gspn_test")
        ti_0 = builder.add_immediate_transition(priority=0, weight=0.5, name="ti_0")
        gspn = builder.build_gspn()
        # todo add some trans and places (place layout after import?)
        #  add arcs

        export_file = os.path.join(str(tmpdir), "gspn.pnpro")

        # export gspn to pnpro
        gspn.export_gspn_pnpro_file(export_file)

        # import gspn
        gspn_parser = stormpy.gspn.GSPNParser()
        gspn_import = gspn_parser.parse(export_file)

        assert gspn_import.get_name() == gspn.get_name()
        assert gspn_import.get_number_of_timed_transitions() == gspn.get_number_of_timed_transitions()
        assert gspn_import.get_number_of_immediate_transitions() == gspn.get_number_of_immediate_transitions()
        assert gspn_import.get_number_of_places() == gspn.get_number_of_places()
        cp_ti_0 = gspn_import.get_immediate_transition("ti_0")
        assert cp_ti_0.get_id() == ti_0


    def test_export_to_pnml(self, tmpdir):
        builder = stormpy.gspn.GSPNBuilder()
        builder.set_name("gspn_test")
        ti_0 = builder.add_immediate_transition(priority=0, weight=0.5, name="ti_0")
        gspn = builder.build_gspn()
        # todo add some trans and places (place layout after import?)
        #  add arcs (cp from above)

        export_file = os.path.join(str(tmpdir), "gspn.pnml")

        # export gspn to pnml
        gspn.export_gspn_pnml_file(export_file)

        # import gspn
        gspn_parser = stormpy.gspn.GSPNParser()
        gspn_import = gspn_parser.parse(export_file)

        assert gspn_import.get_name() == gspn.get_name()
        assert gspn_import.get_number_of_timed_transitions() == gspn.get_number_of_timed_transitions()
        assert gspn_import.get_number_of_immediate_transitions() == gspn.get_number_of_immediate_transitions()
        assert gspn_import.get_number_of_places() == gspn.get_number_of_places()
        cp_ti_0 = gspn_import.get_immediate_transition("ti_0")
        assert cp_ti_0.get_id() == ti_0





