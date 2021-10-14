#define CRT_SECURE_NO_WARNINGS
#include <bangtal>
#include <cstdlib>
#include <ctime>
using namespace bangtal;

ScenePtr scene;
TimerPtr timer;
ObjectPtr intro;
ObjectPtr start, end;
ObjectPtr Dog_bone[12];
ObjectPtr dog;
ObjectPtr bowl;

int bone_x[12];
int rnum;

int index_x(int index) {
	return 865 + 70 * (index % 3);
}

int index_y(int index) {
	return 190 - 20 * (index / 4);
}

void move_bone(int index) {
	Dog_bone[index]->locate(scene, index_x(index)- 610, index_y(index));
	bone_x[index] = index_x(index)-610;
}

int game_index(ObjectPtr object) {
	for (int i = 0; i < 12; i++)
		if (Dog_bone[i] == object)
			return i;
	return -1;
}

void gameStart() {
	srand((unsigned int)time(NULL));
	rnum = rand() % 60 + 5;
	timer->set(rnum); 
	timer->start();
	showTimer(timer);

	start->hide();
	intro->hide();
	dog->setImage("images/sleeping_dog.png");

	for (int i = 0; i < 12; i++) {
		Dog_bone[i]->locate(scene, index_x(i), index_y(i));
	}
}

bool check_end() {
	for (int i = 0; i < 12; i++ ) {
		if (bone_x[i] != index_x(i)-610)
			return false;
	}
	return true;
}

void gameClear() {
	timer->stop();
	start->show();
	showMessage("성공했습니다!!");
}

void gameFail() {
	dog->setImage("images/angry_dog.png");
	showMessage("실패했습니다");
	start->show();	
}

void initGame() {

	scene = Scene::create("밥옮기기게임", "images/background.png");

	dog = Object::create("images/sleeping_dog.png", scene, 670, 270);

	bowl = Object::create("images/bowl.png", scene, 1050, 85);

	char path[20];
	for (int i = 0; i < 12; i++) {
		sprintf_s(path, "images/%d.png", i + 1);
		Dog_bone[i] = Object::create(path, scene, index_x(i), index_y(i));
		Dog_bone[i]->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
			int index = game_index(object);
			move_bone(index);
			if (check_end()) {
				gameClear();
			}
			return true;
			});
	}

	bowl->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
		for (int i = 0; i < 12; i++)
			move_bone(i);
		if (check_end()) {
			gameClear();
		}
		return true;
		});

	dog->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
		gameFail();
		return true;
		});

	intro = Object::create("images/intro.png", scene, 340, 130);
	start = Object::create("images/start.png", scene, 550, 180);

	start->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
		gameStart();
		return true;
		});

	timer = Timer::create(5.f);
	timer->setOnTimerCallback([&](auto timer)->bool {
		gameFail();
		return true;
		});


	startGame(scene);
}
int main() {

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	initGame();

	return 0;
}

