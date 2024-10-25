
#ifndef SEM3LAB1_UI_H
#define SEM3LAB1_UI_H


class UI {
public:
    static void run();
private:

    static void MainMenu();

    static void ArrayMenu();

    static void ListMenu();

    static void ShrdPtrMenu();

    static void UnqPtrMenu();

    static void handleAction(int action);

    static void compareDynamicArrayAndStdVector();

    static void compareLinkedListAndStdList();

    static void compareUnqPtrAndStdUniquePtr();

    static void compareShrdPtrAndStdSharedPtr();
};

#endif //SEM3LAB1_UI_H
