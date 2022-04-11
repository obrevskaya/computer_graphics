import matplotlib.pyplot as plt

plt.figure(1, figsize=(12, 7))

times = []
r = 0
with open("../lab4/time_res_el.txt", "r") as f:

    for line in f:
        times.append(float(line.replace(",", ".")))

plt.suptitle(f"Эллипсы\nПолуось а: 100        Полуось b: 50\nШаг: 10         Кол-во: 500")

y = range(len(times))

labels = ("Каноническое\n уравнение", "Параметрическое\nуравнение", "Брезенхем", "Средняя\nточка")

plt.bar(y, times)
plt.xticks(y, labels)

plt.ylabel(f"Время в мкс.")
plt.show()