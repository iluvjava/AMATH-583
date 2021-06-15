from statistics import mean

DOUBLE_CONSTRUCT_OPT = \
    [138, 139, 136, 136,\
    274, 276, 273, 286, 554,\
    559, 559, 554, 546, 569]
DOUBLE_MUL_OPT = \
    [50, 37, 49, 48, 37,\
    39, 38, 42, 41, 42,\
    115, 77, 114, 78, 76]
FLOAT_CONSTRUCT_OPT = \
    [69, 70, 71, 71, 70,\
    137, 137, 140, 141, 140,\
    277, 277, 283, 274, 277]
FLOAT_MUL_OPT = \
    [9, 9, 10, 10, 9,\
    19, 19, 22, 21, 22,\
    39, 41, 42, 40, 40]
### ----------------------------------------------------------------------------
DOUBLE_CONSTRUCT_NOPT =\
    [147, 150, 162, 166, 163,\
    330, 327, 329, 295, 333,\
    662, 644, 661, 588, 656]
DOUBLE_MUL_NOPT = \
    [50, 37, 49, 48, 37,\
    75, 76, 101, 84, 89,
    188, 153, 200, 152, 190]
FLOAT_CONSTRUCT_NOPT = \
    [80, 81, 106, 108, 102,\
    211, 189, 217, 165, 209,\
    424, 410, 431, 320, 439]
FLOAT_MUL_NOPT = \
    [37, 44, 36, 37, 37,\
    72, 72, 72, 72, 73,\
    189, 182, 191, 150, 186]


def main():

    print("Construction, Nopt, floats over double:")
    print(
        mean(
            II/JJ for II, JJ in 
            zip(FLOAT_CONSTRUCT_NOPT, DOUBLE_CONSTRUCT_NOPT)
            )
        )
    print("Mult with no op, float over double")
    print(
        mean(
            II/JJ for II, JJ in 
            zip(FLOAT_MUL_NOPT, DOUBLE_MUL_NOPT)
        )
    )
    print("construction with op, floats over double")
    print(
        mean(
            II/JJ for II, JJ in 
            zip(FLOAT_CONSTRUCT_OPT, DOUBLE_CONSTRUCT_OPT)
        )
    )
    print("mult with opt, float over double:")
    print(
        mean(
            II/JJ for II, JJ in 
            zip(FLOAT_MUL_OPT, DOUBLE_MUL_OPT)
        )
    )
    print("double mul with and without opt: ")
    print(
        mean(
            II/JJ for II, JJ in zip(DOUBLE_MUL_OPT, DOUBLE_MUL_NOPT)
        )
    )
    

if __name__ == "__main__":
    main()