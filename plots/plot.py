import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

def fitness():
    T1 = 26501
    T2 = 25225
    T4 = 21356
    T8 = 11669
    #T12 = 442
    T16 = 6015
    #T20 = 522
    T24 = 6426
    #X = [ T1/T1, T1/T2, T1/T4, T1/T8, T1/T12, T1/T16, T1/T20, T1/T24  ]
    X = [ T1, T2, T4, T8, T16, T24  ]
    X = np.asarray(X)

    #labels = ['1', '2', '4', '8', '12', '16', '20', '24']
    labels = ['1', '2', '4', '8', '16', '24']

    fig, ax = plt.subplots()
    fig.canvas.draw()
    ax.set_xticklabels(labels)
    plt.plot(X.T)
    plt.ylabel('Fitness')
    plt.xlabel('#Cores')
    plt.title('Fitness per cores')

    # draw vertical line from (70,100) to (70, 250)
    plt.axhline(y=2579)

    plt.show()

def speedup():
    T1 = 3706
    T2 = 2236
    T4 = 1461
    T8 = 1076
    #T12 = 442
    T16 = 810
    #T20 = 522
    T24 = 688
    #X = [ T1/T1, T1/T2, T1/T4, T1/T8, T1/T12, T1/T16, T1/T20, T1/T24  ]
    X = [ T1/T1, T1/T2, T1/T4, T1/T8, T1/T16, T1/T24  ]
    X = np.asarray(X)

    #labels = ['1', '2', '4', '8', '12', '16', '20', '24']
    labels = ['1', '2', '4', '8', '16', '24']

    fig, ax = plt.subplots()
    fig.canvas.draw()
    ax.set_xticklabels(labels)
    plt.plot(X.T)
    plt.ylabel('Speedup')
    plt.xlabel('#Cores')
    plt.title('Speedup graph')
    plt.show()

if __name__ == '__main__':
    #speedup()
    fitness()