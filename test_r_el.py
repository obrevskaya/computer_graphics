import matplotlib.pyplot as plt

timesCannon = []
timesBresenh = []
timesMiddleDot = []
timesParametric = []

with open("../lab4/time_r_res_el.txt", "r") as f:
    a = float(f.readline().replace(",", "."))
    b = float(f.readline().replace(",", "."))
    da = float(f.readline().replace(",", "."))
    db = float(f.readline().replace(",", "."))
    n = int(f.readline())

    for i in range(n):
        timesCannon.append(float(f.readline().replace(",", ".")))
    for i in range(n):
        timesParametric.append(float(f.readline().replace(",", ".")))
    for i in range(n):
        timesBresenh.append(float(f.readline().replace(",", ".")))
    for i in range(n):
        timesMiddleDot.append(float(f.readline().replace(",", ".")))

        
plt.suptitle(f"Эллипсы\nПолуось а: {a}        Полуось b: {b}\nШаг: {da}         Кол-во: {n}")
radiuses = []      
for i in range(n):
    radiuses.append(a + da * i)

plt.plot(radiuses, timesCannon)
plt.plot(radiuses, timesParametric)
plt.plot(radiuses, timesBresenh)
plt.plot(radiuses, timesMiddleDot)
plt.legend(['Каноническое', 'Параметрическое', 'Брезенхем', 'Средняя точка'], loc=2)

plt.ylabel(f"Время в мс.")
plt.xlabel(f"Радиус")
plt.show();