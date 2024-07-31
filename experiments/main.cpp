#include <iostream>

class Stove {
    private:
        int temp = 0;

    public:
        int getTemperature() {
            return temp;
        }

        void setTemperature(int temperature) {
            if (temperature < 0) {
                this->temp = 0;
            }
            else if (temperature > 10) {

            } {

            }
        }
};

int main(){

    Stove stove;

    //stove.temp = 1000000;
    stove.setTemperature(1000000);

    std::cout << "The temperature setting is " << stove.getTemperature() << '\n';

    return 0;
}
