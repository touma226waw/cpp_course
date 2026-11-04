#define main aim_embedded_public_main
#include "../src/main/A1_q1.cpp"
#undef main

void expect(bool ok, const char* name) {
    if (!ok) {
        throw std::runtime_error(std::string("FAIL: ") + name);
    }
    std::cout << "PASS: " << name << '\n';
}

int main() {
    try {
        expect(ratio(10, 0) == -1, "max_hp == 0");
        expect(ratio(-5, 100) == 0, "hp < 0");
        expect(ratio(101, 100) == 100, "hp > max_hp");
        expect(report(1, 60, 100, 59) == "ID=1;HP=60;BAT=59;STATE=WARNING", "WARNING");
        expect(report(2, 60, 0, 60) == "ID=2;HP=INVALID;BAT=60;STATE=OK", "INVALID max_hp");
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
