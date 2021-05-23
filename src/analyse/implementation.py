# Do the python implementation here !


def kolmogorov(histo1, histo2):
    stat, p_value = 0,0
    # TODO
    return (stat, p_value)


def wasserstein(F1, F2, nbr_pixels, p):
    total_area = 0
    delta_D = 0
    cosine_d = 0
    quadratic_d = 0
    for i in range(nbr_pixels):
        if abs(F1[i] - F2[i]) > delta_D:
            delta_D = abs(F1[i] - F2[i])
        total_area += (abs(F1[i]- F2[i]))**p
        cosine_d += F1[i]*F2[i]
        quadratic_d
    return total_area**(1/p), delta_D, 1-cosine_d


def cumulative_fonction(histo1, nbr_pixels):
    result = [0]*histo1
    current_sum = 0

    for i in range(len(histo1)):
        tmp = histo1[i]/nbr_pixels
        result[i] = (tmp + current_sum)*nbr_pixels
        current_sum += tmp
    
    return result

