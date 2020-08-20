#ifndef __model_h__
#define __model_h__
struct Todo {
    char *id = new char[36];
    char *title = new char[36];
    char *ddl = new char[36];

    Todo(char *id, char *title, char *ddl) : id(id), title(title), ddl(ddl) {}
    Todo() {}
};

enum DisplayMode {
    SWIPE_RIGHT,
    SWIPE_LEFT,
    SWIPE_TOP,
    SWIPE_DOWN,
    BREATHING,
    NORMAL
};

enum EspResponse {
    CONNECTING_WIFI = 1,
    WIFI_CONNECTED = 2,
    FETCH_SUCCESS = 3,
    HTTP_NOT_OK = 4,
    REQUEST_FAILED = 5
};

class State {
   private:
    Todo *todoDatas;
    int todoAmount;
    int displayTodoIndex;
    int shouldLightNum;
    DisplayMode LedMode;
    int aniVal;
    int red;
    int green;
    int blue;

   public:
    State();
    Todo getTodoData(int index);
    Todo getDisplayTodo();
    int getDisplayTodoIndex();
    int getTodoAmount();
    int getAniVal();
    DisplayMode getLEDMode();
    uint32_t getColor();
    uint32_t getColorByBrightness(int brightness);
    int getShouldLightNum();
    void setTodo(int index, char *id, char *title, char *ddl);
    void addAniVal();
    void setDisplayTodoIndex(int index);
};
#endif