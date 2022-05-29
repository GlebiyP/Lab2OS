#include <iostream>
#include <thread>
#include <future>

int f(int x)
{
	std::this_thread::sleep_for(std::chrono::seconds(10));
	return x + 1;
}

int g(int x)
{
	//std::this_thread::sleep_for(std::chrono::seconds(10));
	return x * 2;
}

int askContinue()
{
	int cmd;
	do {
		std::cout << "Continue - 1; Stop - 2; Continue and don't ask again - 3\n";
		std::cin >> cmd;
	} while (cmd != 1 && cmd != 2 && cmd != 3);
	return cmd;
}

int main() {
	int x;
	int resF = 0, resG = 0;
	//int res;
	bool gotF = false, gotG = false;
	std::cout << "Enter x: ";
	std::cin >> x;
	auto ObjF = std::async(std::launch::async, f, x);
	auto ObjG = std::async(std::launch::async, g, x);
	std::future_status statusF, statusG;
	auto now = std::chrono::high_resolution_clock::now();
	do {
		if (!gotF) 
			statusF = ObjF.wait_for(std::chrono::microseconds(0));
		if (!gotG) 
			statusG = ObjG.wait_for(std::chrono::microseconds(0));
		auto now2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> ellapsed = now2 - now;
		if (ellapsed.count() >= 5.0)
		{
			now = std::chrono::high_resolution_clock::now();
			int c = askContinue();
			if (c == 2) exit(0);
			if (c == 3) break;
		}
		if (!gotF && statusF == std::future_status::ready && (gotF = true) && (resF = ObjF.get()) == 0 ||
			!gotG && statusG == std::future_status::ready && (gotG = true) && (resG = ObjG.get()) == 0)
		{
			std::cout << "f(" << x << ") * g(" << x << ") = " << 0 << "\n";
			exit(0);
		}
	} while (statusF != std::future_status::ready || statusG != std::future_status::ready);

	resF = !gotF ? ObjF.get() : resF;
	resG = !gotG ? ObjG.get() : resG;

	//res = futureObjF.get() * futureObjG.get();
	std::cout << "f(" << x << ") * g(" << x << ") = " << resF * resG << "\n";
	return 0;
}