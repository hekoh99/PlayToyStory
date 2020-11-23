#include <iostream>
#include <bangtal>
using namespace bangtal;
using namespace std;

ScenePtr startScene;
ScenePtr stage;
ScenePtr stage1;
ScenePtr stage2;
ScenePtr stage3;
ScenePtr explain1;
ScenePtr explain2;
ScenePtr explain2_2;
ScenePtr explain3;
ScenePtr endScene;

ObjectPtr start;
ObjectPtr nextStage;
ObjectPtr HowTobaseball;
ObjectPtr playBaseball;
ObjectPtr playAdventure;
ObjectPtr playFinding;
ObjectPtr endbtn;

ObjectPtr wordSpace;
ObjectPtr wordBalloon;

ObjectPtr Forky;
ObjectPtr Woody;

ObjectPtr F_body;
ObjectPtr F_eyes;
ObjectPtr F_wire;
ObjectPtr F_mouth;

ObjectPtr F_eyesX;
ObjectPtr F_wireX;
ObjectPtr F_mouthX;

ObjectPtr F_eyesO;
ObjectPtr F_wireO;
ObjectPtr F_mouthO;

ObjectPtr alpha[26];
ObjectPtr Punctuation;

int word[4] = { 17, 0, 18, 7 };
int typeLocation = 150;
int answer[4] = { -1, -1, -1, -1 };
int top = -1;
int tryed = 0;

bool BaseballPlaying = false;
bool adventurePlaying = false;
bool findPlaying = false;

// adventure stage 변수들
int WoodyX = 100;
int WoodyY = 50;
int base = 50;
int preKey = 0;

bool isBig = false;

ScenePtr currentScene;
ObjectPtr choco;

// finding 변수들
int score = 0;
TimerPtr stage3Time;
int stage3Fail = 0;

void init_one() {

    WoodyX = 100;
    WoodyY = 50;

    currentScene = stage;
    Forky = Object::create("Images/forky.png");
    Woody = Object::create("Images/woody.png", stage, WoodyX, WoodyY);
    Woody->setScale(0.6f);


    F_body = Object::create("Images/forkyBody.png", stage, 20, 460);
    F_eyes = Object::create("Images/eyes.png", stage2, 1000, 600);
    F_wire = Object::create("Images/wire.png", stage, 850, 550);
    F_mouth = Object::create("Images/mouth.png", stage2, 50, 250);

    F_eyesO = Object::create("Images/haveEyes.png");
    F_mouthO = Object::create("Images/haveMouth.png");
    F_wireO = Object::create("Images/haveArm.png");

    F_eyesX = Object::create("Images/missingEyes.png");
    F_mouthX = Object::create("Images/missingMouth.png");
    F_wireX = Object::create("Images/missingArm.png");

    F_body->setScale(0.4f);
    F_eyes->setScale(0.2f);
    F_wire->setScale(0.2f);

    F_eyesO->defineCombination(F_body, F_eyes);
    F_mouthO->defineCombination(F_body, F_mouth);
    F_wireO->defineCombination(F_body, F_wire);

    F_eyesX->defineCombination(F_mouthO, F_wire);
    F_eyesX->defineCombination(F_mouth, F_wireO);
    F_mouthX->defineCombination(F_eyesO, F_wire);
    F_mouthX->defineCombination(F_eyes, F_wireO);
    F_wireX->defineCombination(F_mouthO, F_eyes);
    F_wireX->defineCombination(F_mouth, F_eyesO);

    Forky->defineCombination(F_wireX, F_wire);
    Forky->defineCombination(F_mouthX, F_mouth);
    Forky->defineCombination(F_eyesX, F_eyes);

    Forky->setOnCombineCallback([&](ObjectPtr ob)->bool {

        Forky->drop();
        Forky->locate(currentScene, 600, 50);
        Forky->setScale(0.4f);
        nextStage->show();
        nextStage->locate(currentScene, 1100, 50);
        nextStage->setScale(0.6f);
        showMessage("포키 탄생!");

        return true;
        });
}

void init_two() {
    for (int i = 0; i < 4; i++) {
        alpha[answer[i]]->hide();
        typeLocation = 150;
        answer[i] = -1;
    }
    top = -1;
    Punctuation->hide();
}

bool checkWin() {
    int ball = 0;
    int str = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (word[i] == answer[j]) {
                if (i == j) str++;
                else ball++;
            }
        }
    }

    string msg = "스트라이크 : " + to_string(str) + " 볼 : " + to_string(ball);
    if (str != 4) {
        showMessage(msg);
    }

    if (str == 4) return true;
    else return false;
}

bool checkNotUsed(int x) {
    if (top == 3) return true;
    for (int i = 0; i < 4; i++) {
        if (answer[i] == x - 1) {
            return false;
        }
    }

    return true;
}

int checkBase() {
    int b = 50;

    if (currentScene == stage) {
        if (WoodyY >= 400 && WoodyX <= 250) b = 440;
        if (WoodyY >= 300 && WoodyX >= 800) b = 330;
        else if (WoodyY >= 300 && WoodyX >= 350 && WoodyX <= 480) b = 340;
        else if (WoodyY >= 250 && WoodyX <= 700 && WoodyX >= 550) b = 250;
        else if (WoodyX >= 400 && WoodyX <= 550 && WoodyY > 140) b = 150;
    }

    else if (currentScene == stage2) {
        if (WoodyY >= 300 && WoodyX <= 300) b = 330;
    }

    return b;
}

int i = 1;

void getSmall() {

    auto stimer = Timer::create(3.0f);
    stimer->setOnTimerCallback([&](TimerPtr t)->bool {
        Woody->setScale(0.6f);
        choco->show();
        isBig = false;
        i = 1;

        return true;
        });
    stimer->start();
}

void getBig() {

    auto timer = Timer::create(0.5f);

    timer->setOnTimerCallback([&](TimerPtr t)->bool {

        if (i < 6) {
            if (i == 5) isBig = true;
            Woody->setScale(0.6f + 0.4f * i);
            t->set(0.5f);
            t->start();
            i++;
        }
        else if (i == 6) {
            getSmall();
        }

        return true;
        });
    timer->start();

}

void pickThings() {
    if (currentScene == stage && WoodyX <= 20 && WoodyY == 440) F_body->pick();
    if (currentScene == stage && WoodyX <= 900 && WoodyX >= 850 && WoodyY >= 430) F_wire->pick();
    if (currentScene == stage2 && WoodyX <= 300 && WoodyX >= 250 && WoodyY >= 430) {
        choco->hide();
        getBig();
    }
    if (currentScene == stage2 && isBig && WoodyX <= 50) F_mouth->pick();
    if (currentScene == stage2 && isBig && WoodyX >= 760) F_eyes->pick();
    if (currentScene == stage2 && WoodyX >= 10 && WoodyX <= 60 && WoodyY == 150) F_mouth->pick();
}

void keyboardCallback(KeyCode x, KeyState k) {

    if (k == KeyState::KEYBOARD_RELEASED && BaseballPlaying) {
        if (x >= 1 && x <= 26 && checkNotUsed(x) || x == 67) {  // 이미 사용한 알파벳 재사용 하지 않게 처리하기
            if (top == 3 && x == 67) {
                if (checkWin()) {
                    Punctuation->show();
                    showMessage("성공!!!");
                    BaseballPlaying = false;
                    nextStage->locate(stage1, 570, 300);
                    nextStage->show();
                }
                else {
                    for (int i = 0; i < 4; i++) {
                        alpha[answer[i]]->hide();
                        typeLocation = 150;
                        answer[i] = -1;
                    }
                    top = -1;
                }
            }
            else if (top < 3 && x >= 1 && x <= 26) {
                alpha[x - 1]->locate(stage1, typeLocation, 540);
                alpha[x - 1]->setScale(1.4f);
                alpha[x - 1]->show();

                top++;
                answer[top] = x - 1;
                typeLocation += 50;

            }
        }
    }

    if (x == 75) {
        if (k == KeyState::KEYBOARD_PRESSED && adventurePlaying) {
            WoodyY += 100;
            Woody->locate(currentScene, WoodyX, WoodyY);
            pickThings();
            preKey = 75;    // 스페이스바를 누른 상태에선 더 멀리 좌 우로 점프하도록 preKey 저장
        }
        if (k == KeyState::KEYBOARD_RELEASED && adventurePlaying) {
            base = checkBase();
            Woody->locate(currentScene, WoodyX, base);
            pickThings();
            WoodyY = base;
            preKey = 0;
        }

    }

    if (x == 83) {
        if (WoodyX > 1250 && currentScene == stage) {
            stage2->enter();
            currentScene = stage2;
            WoodyX = 10;
            Woody->locate(stage2, WoodyX, WoodyY);
            pickThings();
        }
        else if (WoodyX <= 1250) {
            if (k == KeyState::KEYBOARD_PRESSED && adventurePlaying) {
                if (preKey == 75) {                       // 스페이스바를 누른 상태면
                    WoodyX += 100;                        // 더 멀리 이동하라
                }
                else {
                    WoodyX += 50;
                }
                preKey = 83;
                base = checkBase();
                Woody->locate(currentScene, WoodyX, base);
                pickThings();
                WoodyY = base;
            }
        }
    }

    if (x == 82) {
        if (WoodyX < 10 && currentScene == stage2) {
            stage->enter();
            currentScene = stage;
            WoodyX = 1250;
            Woody->locate(stage, WoodyX, WoodyY);
            pickThings();
        }

        else if (WoodyX >= 10) {
            if (k == KeyState::KEYBOARD_PRESSED && adventurePlaying) {
                if (preKey == 75) {
                    WoodyX -= 100;
                }
                else {
                    WoodyX -= 50;
                }
                preKey = 82;
                base = checkBase();
                Woody->locate(currentScene, WoodyX, base);
                pickThings();
                WoodyY = base;
            }
        }

    }

}


int main()
{
    setKeyboardCallback(keyboardCallback);

    startScene = Scene::create("우디와 포키", "Images/startScene.png");
    stage = Scene::create("stage1", "Images/back1.png");
    stage1 = Scene::create("단어 맞추기", "Images/samp.jpg");
    stage2 = Scene::create("stage1", "Images/back2.png");
    stage3 = Scene::create("글자 찾기", "Images/room.png");
    endScene = Scene::create("끝", "Images/end.png");

    explain1 = Scene::create("adventure", "Images/explain1.png");
    explain2 = Scene::create("forky 만들기 성공!", "Images/explain2.png");
    explain2_2 = Scene::create("방법", "Images/explain2_2.png");
    explain3 = Scene::create("방법", "Images/explain3.png");

    start = Object::create("Images/start.png", startScene, 600, 150);
    HowTobaseball = Object::create("Images/howTo.png", explain2, 350, 30);
    playBaseball = Object::create("Images/baseballStart.png", explain2, 650, 30);
    playAdventure = Object::create("Images/adventureStart.png", explain1, 500, 50);
    playFinding = Object::create("Images/findingStart.png", explain3, 500, 30);
    endbtn = Object::create("Images/endbtn.png", endScene, 650, 100);
    endbtn->setScale(0.5f);
    HowTobaseball->setScale(0.7f);
    playBaseball->setScale(0.7f);
    playFinding->setScale(0.7f);

    nextStage = Object::create("Images/next.png");

    wordSpace = Object::create("Images/space.png", stage1, 40, 450);
    wordBalloon = Object::create("Images/speak.png", stage1, 800, 250);
    wordSpace->setScale(0.9f);
    wordBalloon->setScale(0.7f);

    choco = Object::create("Images/choco.png", stage2, 250, 500);
    choco->setScale(0.4f);

    Punctuation = Object::create("Images/mark.png", stage1, 350, 540);
    Punctuation->hide();

    string filename;

    for (int i = 0; i < 26; i++) {
        filename = "Images/alpha/" + to_string(i + 1) + ".png";
        alpha[i] = Object::create(filename);
    }


    nextStage->setOnMouseCallback([&](ObjectPtr ob, int, int, MouseAction) ->bool {
        if (adventurePlaying == true) {
            explain2->enter();
            adventurePlaying = false;

            // 1단계가 끝나고 다음 단계로 넘어가면 이전 단계 상황 리셋 하기
            Woody->hide();
            Forky->hide();
        }
        else if (findPlaying == true) {
            findPlaying = false;
            // 3단계 리셋
            hideTimer();
            alpha[24]->drop();
            alpha[14]->drop();
            alpha[19]->drop();
            score = 0;
            Forky->hide();
            endScene->enter();
            start->show();
            start->locate(endScene, 350, 100);
            start->setScale(0.5f);
            start->setImage("Images/restart.png");
        }
        else {

            explain3->enter();
            //2단계 리셋
            init_two();
        }

        return true;
        });

    playBaseball->setOnMouseCallback([&](ObjectPtr ob, int, int, MouseAction)->bool {
        stage1->enter();
        BaseballPlaying = true;
        playBaseball->locate(explain2, 650, 30);
        return true;
        });

    playAdventure->setOnMouseCallback([&](ObjectPtr ob, int, int, MouseAction)->bool {
        stage->enter();
        adventurePlaying = true;
        init_one();
        return true;
        });

    HowTobaseball->setOnMouseCallback([&](ObjectPtr ob, int, int, MouseAction)->bool {
        explain2_2->enter();
        playBaseball->locate(explain2_2, 500, 30);
        return true;
        });

    playFinding->setOnMouseCallback([&](ObjectPtr ob, int, int, MouseAction)->bool {
        alpha[24]->drop();
        alpha[14]->drop();
        alpha[19]->drop();
        score = 0;
        stage3Time = Timer::create(60.0f);
        stage3Time->setOnTimerCallback([&](TimerPtr t)->bool {
            stage3Fail = 1;
            start->locate(stage3, 550, 100);
            start->setScale(0.5f);
            start->setImage("Images/restart.png");
            start->show();
            findPlaying = false;
            hideTimer();

            showMessage("time out!!");
            return true;
            });
        showTimer(stage3Time);
        stage3Time->start();

        stage3->enter();
        findPlaying = true;
        alpha[24]->locate(stage3, 1128, 423);
        alpha[24]->setScale(0.5f);
        alpha[24]->show();

        alpha[14]->locate(stage3, 625, 267);
        alpha[14]->setScale(0.5);
        alpha[14]->show();

        alpha[19]->locate(stage3, 95, 650);
        alpha[19]->setScale(0.8f);
        alpha[19]->show();

        return true;
        });

    alpha[24]->setOnMouseCallback([&](ObjectPtr ob, int, int, MouseAction)->bool {
        if (findPlaying) {
            alpha[24]->pick();
            score++;
            if (score == 3) {
                stage3Time->stop();
                Forky->locate(stage3, 500, 100);
                Forky->setImage("Images/happyForky.png");
                Forky->setScale(1.5f);
                Forky->show();
                showMessage("I am a toy!!!");
                nextStage->locate(stage3, 1000, 100);
                nextStage->show();
            }
        }

        return true;
        });

    alpha[19]->setOnMouseCallback([&](ObjectPtr ob, int, int, MouseAction)->bool {
        if (findPlaying) {
            alpha[19]->pick();
            score++;
            if (score == 3) {
                stage3Time->stop();
                Forky->locate(stage3, 500, 100);
                Forky->setImage("Images/happyForky.png");
                Forky->setScale(1.5f);
                Forky->show();
                showMessage("I am a toy!!!");
                nextStage->locate(stage3, 1000, 100);
                nextStage->show();
            }
        }

        return true;
        });

    alpha[14]->setOnMouseCallback([&](ObjectPtr ob, int, int, MouseAction)->bool {
        if (findPlaying) {
            alpha[14]->pick();
            score++;
            if (score == 3) {
                stage3Time->stop();
                Forky->locate(stage3, 500, 100);
                Forky->setImage("Images/happyForky.png");
                Forky->setScale(1.0f);
                Forky->show();
                showMessage("I am a toy!!!");
                nextStage->locate(stage3, 1000, 100);
                nextStage->show();
            }
        }

        return true;
        });

    start->setOnMouseCallback([&](ObjectPtr ob, int, int, MouseAction)->bool {
        if (stage3Fail == 1) {
            explain3->enter();
            stage3Fail = 0;
        }
        else {
            explain1->enter();
        }

        start->hide();

        return true;
        });

    endbtn->setOnMouseCallback([&](ObjectPtr ob, int, int, MouseAction)->bool {
        endGame();

        return true;
        });

    bangtal::startGame(startScene);

    return 0;
}