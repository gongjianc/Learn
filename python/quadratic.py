#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import math

#还需要判断b*b - 4ac 是否大于0
def quadratic(a,b,c):
    x1 = float((-b + math.sqrt(b * b - 4 * a * c)) / (2 * a))
    x2 = float((-b - math.sqrt(b * b - 4 * a * c)) / (2 * a))
    return x1,x2

