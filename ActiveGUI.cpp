#include "ActiveGUI.h"

ActiveGUI::ActiveGUI(Texture * texture, int startX, int startY, int endX, int endY, int id) :GUI(texture, startX, startY, endX, endY), CallbackInterface(id) {

}