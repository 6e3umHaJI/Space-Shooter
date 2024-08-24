#pragma once
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "Constants.h"

using namespace sf;

class Table {
protected:
    struct Data {
        Text number, name, score;
    };

    class Row {
    protected:
        RectangleShape row;
        Data data;
        Font font;

    public:
        Row(Vector2f position, Color color, Vector2f size, int delta1, int delta2, int space) {
            font.loadFromFile("fonts/SignikaNegative.ttf");
            row.setFillColor(color);
            row.setSize(size);
            row.setPosition(position);
            Text text("", font, 30);
            text.setFillColor(Color::White);
            text.setPosition(Vector2f(position.x + delta1 - space - 5, position.y));
            data.number = text;
            text.setPosition(Vector2f(position.x + delta1 + delta2 - space - 5, position.y));
            data.name = text;
            text.setPosition(Vector2f(position.x + size.x - 5, position.y));
            data.score = text;
        }
        
        void setData(Data data) {
            this->data.name.setString(data.name.getString());
            this->data.number.setString(data.number.getString());
            this->data.score.setString(data.score.getString());
            this->data.name.setOrigin(this->data.name.getLocalBounds().width, this->data.name.getLocalBounds().height * 0.2);
            this->data.number.setOrigin(this->data.number.getLocalBounds().width, this->data.number.getLocalBounds().height * 0.2);
            this->data.score.setOrigin(this->data.score.getLocalBounds().width, this->data.score.getLocalBounds().height * 0.2);
        }

        Data getData() {
            return data;
        }

        void draw(RenderWindow *window) {
            window->draw(row);
            window->draw(data.number);
            window->draw(data.name);
            window->draw(data.score);
        }

        void setFont(Font font) {
            this->font = font;
            data.name.setFont(this->font);
            data.number.setFont(this->font);
            data.score.setFont(this->font);
        }
    };

    std::string path;
    std::list<Record*> records;
    std::list<Row*> rows;
    std::list<RectangleShape*> columns;
    RectangleShape background;
    int rowHeight, height, width, space, rowAmount;
    int recordAmount, lastRecordIndex, recordAmountMax;
    std::list<Record *>::iterator lastRecord;
    bool isChanged;

public:
    Table() {

    }

    Table(std::string path) {
        this->path = path;
        loadRecords();
        rowHeight = 35;
        rowAmount = 16;
        space = 5;
        height = rowHeight * rowAmount + space * (rowAmount - 1) + 2*space;
        width = height * 1.5;
        background.setSize(Vector2f(width, height));
        background.setFillColor(Color(200, 200, 200, 200));
        background.setOrigin(background.getLocalBounds().width * 0.5, background.getLocalBounds().height * 0.5);
        Vector2f position = Vector2f(1920, 1080);
        background.setPosition(Vector2f(position.x * 0.5, position.y * 0.5 - 100));
        RectangleShape column;
        column.setFillColor(Color(200, 200, 200, 200));
        column.setSize(Vector2f(space, height));
        position = background.getPosition();
        position.x -= width * 0.5 - space;
        position.y -= height * 0.5 - space;
        for (int i = 1; i <= rowAmount; i++)
        {
            rows.push_back(new Row(position, Color(107, 107, 107), Vector2f(width - 2 * space, rowHeight), 70, (width - 70) * 0.5, space));
            position.y += space + rowHeight;
        } 
        Font font;
        font.loadFromFile("fonts/Roboto.ttf");
        rows.front()->setFont(font);
        Data data;
        data.number.setString("#");
        data.name.setString("Nickname");
        data.score.setString("Score");
        rows.front()->setData(data);
        position = background.getPosition();
        position.x -= width * 0.5 - 70;
        position.y -= height * 0.5;
        column.setPosition(position);
        columns.push_back(new RectangleShape(column));
        position.x += (width- 70)*0.5;
        column.setPosition(position);
        columns.push_back(new RectangleShape(column));
        recordAmountMax = 100;
        lastRecordIndex = 1;
        lastRecord = records.begin();
        isChanged = true;
    }

    void loadRecords() {
        recordAmount = 0;
        records.clear();
        std::ifstream file;
        file.open(path);
        if (!file)
            return void();
        Record record;
        while (file.read((char*)(&record), sizeof(Record))) {
            records.push_back(new Record(record));
            recordAmount++;
        }   
        file.close();
    }

    void loadFile() {
        std::ofstream file(path);
        while (!(records.empty())) {
            file.write((char*)(records.front()), sizeof(Record));
            Record* pt = records.front();
            records.pop_front();
            delete pt;
        }
        file.close();
    }

    void addRecord(struct Record record) {
        if (strlen(record.name) == 0)
            return void();
        std::list<Record*>::iterator it;
        it = records.begin();
        while (it != records.end() && strcmp((*it)->name, record.name) != 0)
            it++;
        if (it != records.end())
        {
            if ((*it)->score >= record.score)
                return void();
            Record* pt = *it;
            records.erase(it);
            delete pt;
            recordAmount--;
        }
        it = records.begin();
        while (it != records.end() && (*it)->score >= record.score)
            it++;
        if (recordAmount < recordAmountMax) {  
            records.emplace(it, new Record(record));
            reset();
            recordAmount++;
        }
    }

    void reset() {
        isChanged = true;
        lastRecord = records.begin();
        lastRecordIndex = 1;
    }

    void update(RenderWindow *window) {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            {
                if (event.mouseWheelScroll.delta > 0){
                    if (lastRecordIndex - 1 > 0) {
                        lastRecordIndex--;
                        lastRecord--;
                        isChanged = true;
                    }
                }
                else if(lastRecordIndex + 1 <= recordAmount - rowAmount + 2)
                {             
                    lastRecordIndex++;
                    lastRecord++;
                    isChanged = true;
                }
            }
        }
        if (!isChanged)
            return void();
        std::list<Record*>::iterator itRecord = lastRecord;
        std::list<Row*>::iterator itRow = rows.begin();
        itRow++;
        int i = 0;
        while (itRecord != records.end() && itRow != rows.end()) {
            Data data;
            std::ostringstream dataTemp;
            std::string str = (*itRecord)->name;
            data.name.setString(str);
            dataTemp.str("");
            dataTemp << (*itRecord)->score;
            data.score.setString(dataTemp.str());
            dataTemp.str("");
            dataTemp << lastRecordIndex + i;
            data.number.setString(dataTemp.str());
            (*itRow)->setData(data);
            i++;
            itRow++;
            itRecord++;
        }
        isChanged = false;
    }

    void draw(RenderWindow *window) {
        window->draw(background);
        for(std::list<Row*>::iterator it = rows.begin(); it != rows.end(); it++)
            (*it)->draw(window);
        for (std::list<RectangleShape*>::iterator it = columns.begin(); it != columns.end(); it++)
            window->draw(**it);
    }
};