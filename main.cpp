/*
TITLE:
    main.cpp

BRIEF:
    header_docu (main.cpp)

DESC:
    header_docu is a small nafty C++ commandline tool to read the first comment block of a textfile and outputs it as HTML5- or markdown- snippet    

    DEPENDENCIES:
        cxxopts
        a lightweight C++ option parser library, supporting the standard GNU style syntax for options.
        https://github.com/jarro2783/cxxopts

SOURCE:
    https://github.com/Zheng-Bote/header_docu

SYNTAX:
    header_docu -h | header_docu --help
    
    -c, --css arg   include external CSS file for html output (default: false)
    -d, --dir arg   parse directory (default: .)
    -e, --ext arg   file extension (default: h)
    -f, --file arg  1 single textfile
    -o, --out arg   output type md | html (default: md)
    -h, --help      Print usage
    -v, --version   Version

    EXAMPLES:
    /usr/local/bin/header_docu --dir . --ext h --out md // read/parse all *.h files in the current directory
    /usr/local/bin/header_docu --file ~/DEV/CPP/header_docu/main.cpp --out html // read/parse single file and output as HTML5 snippet

RETURN:
    output.md | output.html
    void

HISTORY:
Version | Date       | Developer        | Comments
------- | ---------- | ---------------- | ---------------------------------------------------------------
0.1.0   | 2022-03-05 | RZheng           | created 
0.1.1   | 2022-03-06 | RZheng           | fixed markdown title 

*/

#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <QRegularExpression>

#include <cxxopts.hpp>

const QString VERSION = "v0.1.1";

bool mdOutput = false;

void writeFile(QString text) {
    QString outFile;
    if(! mdOutput) {
        outFile = "./output.html";
    }
    else {
        outFile = "./output.md";
    }
    QFile file(outFile);

    if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << text;
        file.close();
    }
}

QString parseLine(QString text) {
    QRegularExpression searchTitle("TITLE:");
    QRegularExpression searchBrief("BRIEF:");
    QRegularExpression searchDesc("DESC:");
    QRegularExpression searchSource("SOURCE:");
    QRegularExpression searchSyntax("SYNTAX:");
    QRegularExpression searchReturn("RETURN:");
    QRegularExpression searchHistory("HISTORY:");

    QRegularExpressionMatch matchSearchStr = searchTitle.match(text);
    if(matchSearchStr.hasMatch()) {
        if(! mdOutput) {
            text = "  <div class=\"title\">" + text;
        } else {
            text = "\n- - -\n## " + text;
        }
        return text;
    }

    matchSearchStr = searchBrief.match(text);
    if(matchSearchStr.hasMatch()) {
        if(! mdOutput) {
            text = "  <div class=\"brief\">" + text;
        } else {
            text = "### " + text;
        }
        return text;
    }

    matchSearchStr = searchDesc.match(text);
    if(matchSearchStr.hasMatch()) {
        if(! mdOutput) {
            text = "  <div class=\"desc\">" + text;
        } else {
            text = "### " + text;
        }
        return text;
    }

    matchSearchStr = searchSource.match(text);
    if(matchSearchStr.hasMatch()) {
        if(! mdOutput) {
            text = "  <div class=\"source\">" + text;
        } else {
            text = "### " + text;
        }
        return text;
    }

    matchSearchStr = searchSyntax.match(text);
    if(matchSearchStr.hasMatch()) {
        if(! mdOutput) {
            text = "  <div class=\"syntax\">" + text;
        } else {
            text = "### " + text;
        }
        return text;
    }

    matchSearchStr = searchReturn.match(text);
    if(matchSearchStr.hasMatch()) {
        if(! mdOutput) {
            text = "  <div class=\"return\">" + text;
        } else {
            text = "### " + text;
        }
        return text;
    }

    matchSearchStr = searchHistory.match(text);
    if(matchSearchStr.hasMatch()) {
        if(! mdOutput) {
            text = "  <div class=\"history\">" + text;
        } else {
            text = "### " + text;
        }
        return text;
    }

    if(! mdOutput) {
        return("    <div class=\"content\"><pre>" + text + "</pre></div>");
    } else {
        return("> " + text);
    }
}

void readFile(QString pathWithFile) {
    QFile file(pathWithFile);
    QString searchBegin = QRegularExpression::escape("/*");
    QRegularExpression reBegin(searchBegin);
    QString searchEnd = QRegularExpression::escape("*/");
    QRegularExpression reEnd(searchEnd);

    QString outText;
//    QTextStream out(stdout);

    // QFile file2("/home/zb_bamboo/TEMP/ESP32_BME680_Webserver_MQTT-Client/rz_version.h");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    QString line;
    bool isComment = false;

    while (in.readLineInto(&line)) {
        //out << line << "\n";
        QRegularExpressionMatch matchStart = reBegin.match(line);
        QRegularExpressionMatch matchEnd = reEnd.match(line);
        if(matchStart.hasMatch()) {
            isComment = true;
            if(! mdOutput) {
                outText = "<div class=\"header\">\n  <br/>\n";
            }
            continue;
        }
        if(matchEnd.hasMatch()) {
           isComment = false;
           if(! mdOutput) {
                outText.append("</div>\n");
           }
           break;
        }
        if(isComment) {
            if(line.isEmpty()) {
                if(! mdOutput) {
                    outText.append("  </div>\n  <br/>");
                }
            }
            else {
//                out << line << "\n";
                outText.append(parseLine(line));
            }
            outText.append("\n");
        }
        else {
            break;
        }
    }

    file.close();

    writeFile(outText);

}

void readDir( QString dir, QString ext) {
    QTextStream out(stdout);
    //QString pathToImg = "/home/zb_bamboo/TEMP/ESP32_BME680_Webserver_MQTT-Client/";
    QDir directory(dir);
    QStringList files = directory.entryList(QStringList() << "*." + ext, QDir::Files);

    foreach(QString filename, files)
    {
       QFileInfo info(dir + "/" + filename);
        out << filename << " ";
       out << info.size() / 1024 << "\n";
       readFile(dir + filename);
     }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    cxxopts::Options options("header_parser", "parse header and output as docu");

    options.add_options()
        ("c,css", "include external CSS file for html output", cxxopts::value<std::string>()->default_value("false"))
        ("d,dir", "parse directory", cxxopts::value<std::string>()->default_value("."))
        ("e,ext", "file extension", cxxopts::value<std::string>()->default_value("h"))
        ("f,file", "1 single textfile", cxxopts::value<std::string>())
        ("o,out", "output type md | html", cxxopts::value<std::string>()->default_value("html"))
        ("h,help", "Print usage")
        ("v,version", "Version")
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
      std::cout << options.help() << std::endl;
      exit(0);
    }

    if (result.count("version"))
    {
      std::cout << VERSION.toStdString() << std::endl;
      exit(0);
    }

    if (result.count("out"))
    {
      std::string out = result["out"].as<std::string>();
      if(out.compare("md") != 0) {
            std::cout << "out != 0 false " << out << std::endl;
            mdOutput = false;
      }
      else {
          std::cout << "mdout == 0 true" << std::endl;
          mdOutput = true;
      }

    }

    if(! mdOutput) {
        QString htmlStart = R"(<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="author" content="header_docu">
    <title>header docu</title>)";

        writeFile(htmlStart);

        QString css = R"(
    <style>
      .header {
        margin: 5px 0 5px 0;
        border: 1px solid lightgrey;
        border-radius: 5px;
        padding: 0 15px 0 15px;
      }
      .title {
        width: 100%;
      }
      .title .content {
        background-color: lightgrey;
        font-weight: bold;
      }
    </style>
  </head>
  <body id="body">
    )";
        writeFile(css);
    }

    if (result.count("dir") && result.count("file")) {
        readFile(QString::fromStdString(result["dir"].as<std::string>()) + "/" + QString::fromStdString(result["file"].as<std::string>()));
    }
    else if (result.count("dir") && result.count("ext")) {
        readDir(QString::fromStdString(result["dir"].as<std::string>()) + "/", QString::fromStdString(result["ext"].as<std::string>()));
    }

    if(! mdOutput) {
        QString htmlEnd = R"(
  </body>
</html>
    )";
        writeFile(htmlEnd);
    }

    //return a.exec();
    QCoreApplication::exit();
}
