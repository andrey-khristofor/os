#include <iostream>
#include <thread>
#include <future>

bool f(int x)
{
    std::this_thread::sleep_for(std::chrono::seconds(7));
    return x % 2 == 0;
}

bool g(int x)
{
    return x == 11;
}

std::string askIfCont()
{
    std::string command;
    do
    {
        std::cout << "Continue(c)/Stop(s)/ContinueDontAskAgain(cdaa): ";
        std::cin >> command;
    } while (command != "c" && command != "s" && command != "cdaa");
    return command;
}

int main() {
    int x;
    std::cout << "Enter x: ";
    std::cin >> x;
    auto futureObjF = std::async(std::launch::async, f, x);
    auto futureObjG = std::async(std::launch::async, g, x);
    std::future_status statusF, statusG;
    auto now = std::chrono::high_resolution_clock::now();
    do
    {
        statusF = futureObjF.wait_for(std::chrono::milliseconds(0));
        statusG = futureObjG.wait_for(std::chrono::milliseconds(0));
        auto now2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> ellapsed = now2 - now;
        if (ellapsed.count() >= 5.0)
        {
            now = std::chrono::high_resolution_clock::now();
            std::string c = askIfCont();
            if (c == "s") exit(0);
            if (c == "cdaa") break;
        }

    } while (statusF != std::future_status::ready || statusG != std::future_status::ready);
    std::string resp = futureObjF.get() || futureObjG.get() ? " true" : " false";
    std::cout << "f(" << x << ") || g(" << x << ") = " << resp << "\n";
    return 0;
}