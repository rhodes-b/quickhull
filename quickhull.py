# quickhull.py
# by: Brice Rhodes


def quickhull(points):
    indices = range(len(points))
    min_i = min(indices, key=points.__getitem__)
    max_i = max(indices, key=points.__getitem__)
    # set of points without min or max
    s = [points[i] for i in range(len(points)) if i != min_i and i != max_i]
    # calculate determs
    dets = [determ(points[min_i], points[max_i], p) for p in s]
    # only want the points left of the line
    s1 = [s[i] for i in range(len(s)) if dets[i] > 0]
    top = halfhull(points[min_i], points[max_i], s1, [n for n in dets if n > 0])
    # only want the points right of the line but flip sign of dets so its technically left
    s2 = [s[i] for i in range(len(s)) if dets[i] < 0]
    bottom = halfhull(points[max_i], points[min_i], s2, [-n for n in dets if n < 0])
    print(f"TOP: {top}")
    print(f"BOTTOM: {bottom}")
    return top + bottom


def halfhull(p1, p2, s, determs):
    if not s:
        print(p1, p2)
        return [[p1, p2]]
    indices = range(len(determs))
    # p_max is magnitude but since we know all dets > 0 we can just do max
    p_max = s[max(indices, key=determs.__getitem__)]
    # points left of p1 -> p_max
    s1 = [p for p in s if determ(p1, p_max, p) > 0]
    s1_det = [determ(p1, p_max, p) for p in s1]
    # points left of p_max -> p2
    s2 = [p for p in s if determ(p_max, p2, p) > 0]
    s2_det = [determ(p_max, p2, p) for p in s2]

    return halfhull(p1, p_max, s1, s1_det) + halfhull(p_max, p2, s2, s2_det)


def determ(p1, p2, p3):
    """det = x1*y2 + x3*y1 + x2*y3 − x3*y2 − x2*y1 − x1*y3"""
    x1, y1 = p1
    x2, y2 = p2
    x3, y3 = p3
    return x1*y2 + x3*y1 + x2*y3 - x3*y2 - x2*y1 - x1*y3


if __name__ == "__main__":
    # personal example
    points = [(0, 3), (1, 0), (2, 2), (3, 1), (4, 4)]
    hull = quickhull(points)
    print(hull)

    # generated using [(randint(0, 20), randint(0, 20) for _ in range(15)]
    # points = [(2, 16), (5, 19), (7, 4), (5, 3), (13, 4),
    #          (2, 2), (15, 11), (5, 19), (12, 9), (4, 12),
    #          (13, 14), (7, 15), (7, 5), (18, 12), (4, 2)]

    # hull = quickhull(points)
    # print(hull)
    """
    using demos to plot the points I've confirmed it
    generates the correct convex hull by eye
    """


