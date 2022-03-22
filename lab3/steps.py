import matplotlib
import matplotlib.pyplot as plt
from itertools import count, takewhile
import pylab
def frange(start, stop, step):
    return takewhile(lambda x: x< stop, count(start, step))

plt.figure(figsize=(20, 10))

r = 0
angle_step = 0
    
with open("../lab3/steps_res.txt", "r") as f:
    r = float(f.readline().replace(",", "."))
    angle_step = float(f.readline().replace(",", "."))
    max_angle = float(f.readline().replace(",", "."))
    steps = []
    for line in f:
        steps.append(float(line.replace(",", ".")))

plt.suptitle(f"Анализ ступенчатости\nДлина отрезка - {r}")

angles = list(frange(0, max_angle, angle_step))

x = 12
y = 8

plt.subplot(2, 3, 1)
plt.title("ЦДА", fontsize=x)
plt.xlabel("Угол наклона прямой", fontsize=y)
plt.ylabel(f"Число ступеней", fontsize=y)
plt.grid(True)
plt.plot(angles, steps[:len(angles)])

plt.subplot(2, 3, 2)
plt.title("Брезенхем целочисленный", fontsize=x)
plt.xlabel("Угол наклона прямой", fontsize=y)
plt.ylabel(f"Число ступеней", fontsize=y)
plt.grid(True)
plt.plot(angles, steps[len(angles):2*len(angles)])

plt.subplot(2, 3, 3)
plt.title("Брезенхем действительный", fontsize=x)
plt.xlabel("Угол наклона прямой", fontsize=y)
plt.ylabel(f"Число ступеней", fontsize=y)
plt.grid(True)
plt.plot(angles, steps[2*len(angles):3*len(angles)])

plt.subplot(2, 3, 4)
plt.title("Брезенхем сглаживающий", fontsize=x)
plt.xlabel("Угол наклона прямой", fontsize=y)
plt.ylabel(f"Число ступеней", fontsize=y)
plt.grid(True)
plt.plot(angles, steps[3*len(angles):4*len(angles)])

plt.subplot(2, 3, 5)
plt.title("Ву", fontsize=x)
plt.xlabel("Угол наклона прямой", fontsize=y)
plt.ylabel(f"Число ступеней", fontsize=y)
plt.grid(True)
plt.plot(angles, steps[4*len(angles):5*len(angles)])

plt.show()


