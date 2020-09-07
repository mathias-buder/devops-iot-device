# *********************************************************************
#
#       SEWELA owns the copyright in this document and associated
#       documents and all rights are reserved. These documents must
#       not be used for any purpose other than that for which they
#       are supplied and must not be copied in whole or in part or
#       disclosed to others without prior written consent of SEWELA.
#       Any copy of this drawing or document made by any method
#       must also include a copy of this legend.
#
#       (c) SEWELA 2020
#
# *********************************************************************/

idf_component_register(SRCS "assignment.c
							"ConfRadVid.c"
							"coordinate_transformations.c"
							"filter_utility.c"
							"KalFil1D.c"
							"KalFilChk.c"
							"KalFilChk_Cfg.c"
							"KalFilGen.c"
							"least_squares.c"
							"linear_buffer.c"
							"matrix_manipulation.c"
							"ModCAA.c"
							"ModCACV.c"
							"newton_raphson.c"
							"polynomial_line.c"
							"queue.c"
							"ring_structure.c"
							"vector_manipulation.c"
                       INCLUDE_DIRS ".")

