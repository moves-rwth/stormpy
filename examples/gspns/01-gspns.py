import stormpy
import stormpy.gspn

import stormpy.examples
import stormpy.examples.files


def example_gspns_01():
    # construct gspn with gspn builder
    builder = stormpy.gspn.GSPNBuilder()
    builder.set_name("gspn")

    it_0 = builder.add_immediate_transition(1, 0.0, "it_0")
    it_layout = stormpy.gspn.LayoutInfo(1.5, 1.5)
    builder.set_transition_layout_info(it_0, it_layout)

    tt_0 = builder.add_timed_transition(0, 0.4, "tt_0")
    tt_layout = stormpy.gspn.LayoutInfo(12.5, 1.5)
    builder.set_transition_layout_info(tt_0, tt_layout)

    place_0 = builder.add_place(1, 1, "place_0")
    p0_layout = stormpy.gspn.LayoutInfo(6.5, 1.5)
    builder.set_place_layout_info(place_0, p0_layout)

    place_1 = builder.add_place(1, 0, "place_1")
    p1_layout = stormpy.gspn.LayoutInfo(18.5, 1.5)
    builder.set_place_layout_info(place_1, p1_layout)

    # arcs
    builder.add_output_arc(it_0, place_0)
    builder.add_inhibition_arc(place_0, it_0)
    builder.add_input_arc(place_0, tt_0)
    builder.add_output_arc(tt_0, place_1)

    # build gspn
    gspn = builder.build_gspn()
    print("GSPN with {} places, {} immediate transitions and {} timed transitions.".format(gspn.get_number_of_places(),
                                                                                           gspn.get_number_of_immediate_transitions(),
                                                                                           gspn.get_number_of_timed_transitions()))

    path = stormpy.examples.files.gspn_pnpro_simple

    # export gspn to pnpro format
    gspn.export_gspn_pnpro_file(path)

    # import gspn
    gspn_parser = stormpy.gspn.GSPNParser()
    gspn_import = gspn_parser.parse(path)

    print("GSPN with {} places, {} immediate transitions and {} timed transitions.".format(
        gspn_import.get_number_of_places(), gspn.get_number_of_immediate_transitions(),
        gspn.get_number_of_timed_transitions()))


if __name__ == '__main__':
    example_gspns_01()
