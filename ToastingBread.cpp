#define _CRT_SECURE_NO_WARNINGS
#include <bangtal>
#include <cstdlib>
#include <ctime>
using namespace bangtal;

ScenePtr scene;
ObjectPtr bread, toast, burnt, startbutton, replaybutton, endbutton;
TimerPtr timer;
ObjectPtr toastingbread[10];
SoundPtr bgm, gameoversd, successsd;


//버튼클릭효과음
void uisound() {
	SoundPtr uisound = Sound::create("Sounds/uisound.ogg");
	uisound->play();
}

//재도전, 끝내기 버튼
void button() {
	replaybutton->setOnMouseCallback([&](auto object, int x, int y, auto action)->bool {
		gameoversd->stop();
		successsd->stop();
		uisound();
		bgm->play(true);

		startgame();
		replaybutton->hide();
		endbutton->hide();

		return true;
		});

	endbutton->setOnMouseCallback([&](auto object, int x, int y, auto action)->bool {
		uisound();
		endGame();

		return true;
		});
}

//랜덤으로 식빵 섞기
void mixbread(void) {
	srand((unsigned int)time(NULL));

	char breadnum[20];
	int x, y, num;
	int checknum[10] = {10,10,10,10,10,10,10,10,10,10};
	

	for(int i = 0; i < 10; i++){
		re:
		num = rand() % 10;
	
		for (int j = 0; j < i + 1; j++) {
			if (num == checknum[j]) {
				goto re;
			}
		}

		checknum[i] = num;

		switch (num) {
			case 0:
				x = 60;
				y = 370;
				break;
			case 1:
				x = 300;
				y = 370;
				break;
			case 2:
				x = 540;
				y = 370;
				break;
			case 3:
				x = 780;
				y = 370;
				break;
			case 4:
				x = 1020;
				y = 370;
				break;
			case 5:
				x = 60;
				y = 100;
				break;
			case 6:
				x = 300;
				y = 100;
				break;
			case 7:
				x = 540;
				y = 100;
				break;
			case 8:
				x = 780;
				y = 100;
				break;
			case 9:
				x = 1020;
				y = 100;
				break;
		}
		sprintf(breadnum, "Images/bread%d.png", i + 1);
		toastingbread[i] = Object::create(breadnum, scene, x, y);
	}
}

//뒤집기 성공
void success() {
	bgm->stop();
	successsd->play();

	timer->stop();
	for (int i = 0; i < 10; i++) {
		toastingbread[i]->setImage("Images/smiletoast.png");
	}
	showMessage("식빵이 맛있게 구워졌어요!");

	replaybutton = Object::create("Images/restartbutton.png", scene, 300, 300);
	endbutton = Object::create("Images/endbutton.png", scene, 760, 300);

	button();
}


//시간초과-실패
void gameover() {
	bgm->stop();
	gameoversd->play();

	for (int i = 0; i < 10; i++) {
		toastingbread[i]->setImage("Images/burnt.png");
	}
	showMessage("식빵이 다 타버렸어요!");

	replaybutton = Object::create("Images/restartbutton.png", scene, 300, 300);
	endbutton = Object::create("Images/endbutton.png", scene, 760, 300);

	button();
}

//식빵 클릭
void breadclick(int i) {
	toastingbread[i]->setImage("Images/toast.png");
	SoundPtr flipbread = Sound::create("Sounds/flipbread.ogg");
	flipbread->play();
}

//식빵 뒤집기
//(루프나 조건문으로 간단히 하고 싶었으나, 자꾸 예외처리 및 오류가 떠 부득이하게 아래와 같이 씀)
void breadflip() {
	toastingbread[0]->setOnMouseCallback([&](auto object, int x, int y, MouseAction)->bool {
		breadclick(0);
		
		toastingbread[1]->setOnMouseCallback([&](auto object, int x, int y, MouseAction)->bool {
			breadclick(1);
			
			toastingbread[2]->setOnMouseCallback([&](auto object, int x, int y, MouseAction)->bool {
				breadclick(2);

				toastingbread[3]->setOnMouseCallback([&](auto object, int x, int y, MouseAction)->bool {
					breadclick(3);

					toastingbread[4]->setOnMouseCallback([&](auto object, int x, int y, MouseAction)->bool {
						breadclick(4);

						toastingbread[5]->setOnMouseCallback([&](auto object, int x, int y, MouseAction)->bool {
							breadclick(5);

							toastingbread[6]->setOnMouseCallback([&](auto object, int x, int y, MouseAction)->bool {
								breadclick(6);

								toastingbread[7]->setOnMouseCallback([&](auto object, int x, int y, MouseAction)->bool {
									breadclick(7);

									toastingbread[8]->setOnMouseCallback([&](auto object, int x, int y, MouseAction)->bool {
										breadclick(8);

										toastingbread[9]->setOnMouseCallback([&](auto object, int x, int y, MouseAction)->bool {
											breadclick(9);
											success();
											return true;
											});
										return true;
										});
									return true;
									});
								return true;
								});
							return true;
							});
						return true;
						});
					return true;
					});
				return true;
				});
			return true;
			});
		return true;
		});
}


//게임 시작
void startgame() {
	scene->setImage("Images/playing.png");
	startbutton->hide();
	bread->hide();
	toast->hide();
	burnt->hide();

	mixbread();

	timer = Timer::create(10.f);
	showTimer(timer);
	timer->start();

	breadflip();

	timer->setOnTimerCallback([&](TimerPtr)->bool {
		gameover();

		return true;
		});
}


//시작화면
void readypage() {
	scene = Scene::create("식빵굽기", "Images/background.png");

	bread = Object::create("Images/bread.png", scene, 465, 200);
	toast = Object::create("Images/toast.png", scene, 100, 200, false);
	burnt = Object::create("Images/burnt.png", scene, 850, 200, false);
	startbutton = Object::create("Images/startbutton.png", scene, 515, 50);

	toast->setScale(1.7f);
	toast->show();

	burnt->setScale(1.65f);
	burnt->show();

	startbutton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		uisound();
		startgame();
		return true;
		});

	startGame(scene);
}


int main(){
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	
	gameoversd = Sound::create("Sounds/gameover.mp3");
	successsd = Sound::create("Sounds/success.mp3");
	bgm = Sound::create("Sounds/bgm.mp3");
	bgm->play(true);

	readypage();

	return 0;
}