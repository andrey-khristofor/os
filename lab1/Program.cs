using System;
using System.Threading;

namespace lab1
{
    class Fork
    {
        bool[] fork = new bool[5];

        public void Get(int left, int right)
        {
            lock (this)
            {
                while (fork[left] || fork[right]) Monitor.Wait(this);
                fork[left] = true; fork[right] = true;
            }
        }

        public void Put(int left, int right)
        {
            lock (this)
            {
                fork[left] = false; fork[right] = false;
                Monitor.PulseAll(this);
            }
        }
    }
    class Philosopher
    {
        int n;
        int thinkDelay;
        int eatDelay;
        int left, right;
        Fork fork;

        public Philosopher(int n, int thinkDelay, int eatDelay, Fork fork)
        {
            this.n = n;
            this.thinkDelay = thinkDelay;
            this.eatDelay = eatDelay;
            this.fork = fork;
            left = n == 0 ? 4 : n - 1;
            right = (n + 1) % 5;
            new Thread(new ThreadStart(Run)).Start();
        }

        public void Run()
        {
            while (true)
            {
                try
                {
                    Console.WriteLine("Philosopher " + n + " is thinking...");
                    Thread.Sleep(thinkDelay);
                    Console.WriteLine("Philosopher " + n + " is hungry...");
                    fork.Get(left, right);
                    Console.WriteLine("Philosopher " + n + " is eating...");
                    Thread.Sleep(eatDelay);
                    fork.Put(left, right);
                }
                catch
                {
                    return;
                }
            }
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            Fork fork = new Fork();
            new Philosopher(0, 500, 500, fork);
            new Philosopher(1, 500, 500, fork);
            new Philosopher(2, 500, 500, fork);
            new Philosopher(3, 500, 500, fork);
            new Philosopher(4, 500, 500, fork);
        }
    }
}