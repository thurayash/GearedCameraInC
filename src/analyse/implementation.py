# Do the python implementation here !


def kolmogorov(histo1, histo2):
    stat, p_value = 0,0
    # TODO
    return (stat, p_value)



def wasserstein(histo1, histo2):
    distance = 0
    for i in range(len(histo1)):
        distance = max(distance, abs(histo1[i] - histo2[i]))
    return distance