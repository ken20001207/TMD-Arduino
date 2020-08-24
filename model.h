#ifndef __model_h__
#define __model_h__
struct Todo {
    char *title = new char[36];
    char *ddl = new char[36];
    char *status = new char[36];

    Todo(char *title, char *ddl, char *status)
        : title(title), ddl(ddl), status(status) {}
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
    DisplayMode getLEDMode();
    int getDisplayTodoIndex();
    int getTodoAmount();
    int getAniVal();
    uint32_t getColor();
    uint32_t getColorByBrightness(int brightness);
    int getShouldLightNum();

    void setLEDMode(DisplayMode mode);
    void setShouldLightNum(int num);
    void setTodo(int index, char *id, char *title, char *ddl);
    void setDisplayColor(int red, int green, int blue);
    void setDisplayTodoIndex(int index);

    void addAniVal();
    void updateLEDs();

    void nextTodo();
    void prevTodo();
};
#endif