import matplotlib.pyplot as plt

plt.figure(1, figsize=(10, 7))

times = []
r = 0
with open("../lab3/time_res.txt", "r") as f:
    r = float(f.readline().replace(",", "."))

    for line in f:
        times.append(float(line.replace(",", ".")) * 1000000)

y = range(len(times))

labels = ("ЦДА", "Брезенхем\nцелочисленный", "Брезенхем\nдействительный", "Брезенхем\nсглаживающий", "Ву")

plt.bar(y, times)
plt.xticks(y, labels)

plt.ylabel(f"t (мкс.)\nДлина отрезка - {r}")
plt.show()