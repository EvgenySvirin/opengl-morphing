#include <QApplication>
#include <mainwindow.h>
#include <QCommandLineParser>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    bool isAnimated = false;
    float t = 0.f;

    QCommandLineParser parser;
    parser.addHelpOption();


    parser.setApplicationDescription("Application displaying morphing rotating cube into sphere.");

    QCommandLineOption startingTime ("t",
         QCoreApplication::translate("main", "Time of morphing process."),
         QCoreApplication::translate("main", "Integer number from 0 to 100."));
    parser.addOption(startingTime);

    QCommandLineOption animation   ("a",
        QCoreApplication::translate("main", "Animate or slide the morphing process."),
        QCoreApplication::translate("main", "true or false."));
    parser.addOption(animation);
    parser.process(app);


    if (parser.isSet(animation)) {
        isAnimated = parser.values(animation).first() == "true";
    }

    if (parser.isSet(startingTime)) {
        t = parser.values(startingTime).first().toFloat();
    }

    MainWindow widget(nullptr, static_cast<int>(t), isAnimated);
    widget.show();

    return app.exec();
}
