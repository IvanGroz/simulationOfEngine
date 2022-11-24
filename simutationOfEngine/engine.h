#pragma once
class engine
{
private:
	float environmentTemp = 20.0f; //температура окружающей среды
	float engineTemp = 20.0f; //температура двигателя 
	float I = 0.1f;//Момент инерции двигателя I (кг∙м^2)
	float overheatTemp = 110.0f;//Температура перегрева (цельсия)
	float Hm = 0.01f;//Коэффициент зависимости скорости нагрева от крутящего момента Hm (цельсия/𝐻∙𝑚∙сек)
	float Hv = 0.0001f;//Коэффициент зависимости скорости нагрева от скорости вращения коленвала Hv ((цельсия∙сек)/рад^2)
	float C = 0.1f; // Коэффициент зависимости скорости охлаждения от температуры двигателя и окружающей среды C (1/сек)
	float V = 0.0f; //Скорость вращения коленвала (рад/сек)
	float M = 20.0f; //крутящий момент (Н*м)
	float a = M / I; //ускорение коленвала
	float velHeat = M * Hm + (V * V) * Hv; //Скорость нагрева двигателя (цельсия/сек)
	float velCool = C * (environmentTemp - engineTemp); //Скорость охлаждения (цельсия/сек)

	float Mvalue[6] = {20.0f ,75.0f,100.0f,105.0f,75.0f,0.0f };//значения соответствия М и V
	float Vvalue[6] = { 0.0f, 75.0f, 150.0f, 200.0f, 250.0f, 300.0f };

	bool simulationState = false;//Состояние симуляции

	float timer = 0.0f; //время симуляции
public:
	engine() {}
	engine(float envTemp) {
		environmentTemp = engineTemp = envTemp; //Температура двигателя до момента старта должна равняться температуре окружающей среды.
	}
	float calcTempVel() {//Рассчет итоговой скорости изменения температуры
		return velHeat - velCool;
	}
	void simulation(float deltaTime);
	void update();//перерасчет характеристик во времени
	void start();
	void stop();
	float getSimulationTime();
	float getEnvTemp();
};

 void engine::simulation(float deltaTime)
{

	 while (simulationState) {
		V += (a*deltaTime);//т.к. эти переменные напрямую зависят от времени в секундах,
						  //то от минимальной еденицы времени симуляции зависит величина изменения 
		engineTemp += (calcTempVel() * deltaTime);
		
		for (int i = 0; i < (sizeof(Vvalue) / sizeof(float)); i++) {
			if (V >= Vvalue[i]) {
				if (V < Vvalue[i + 1])//если попадает в промежуток между
				{
					M = Mvalue[i];
				}
			}

		}

		if (engineTemp >= overheatTemp) {//двигатель перегрелся
			stop();
		}
		update();
		timer+=deltaTime;
	}

}

 void engine::update()
{
	velHeat = M * Hm + (V * V) * Hv;
	velCool = C * (environmentTemp - engineTemp); 
	a = M / I;
}

void engine::start() {
	timer = 0.0f;
	simulationState = true;
	simulation(0.001f);
}

 void engine::stop()
 {
	 engineTemp = environmentTemp;
	 V = 0.0f;
	 M = 20.0f;
	 update();
	 simulationState = false;
}

  float engine::getSimulationTime()
 {
	 return timer;
 }

  float engine::getEnvTemp()
  {
	  return environmentTemp;
  }
