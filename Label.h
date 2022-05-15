#ifndef LABEL_H
#define LABEL_H

class Label {
    char *name;
    bool declare = false;
    int position;
public:
    Label();

    char *get_name() const;
    void set_name(const char *n);

    bool get_declare() const;
    void set_declare();

    int get_position() const;
    void set_position(int p);
};

#endif //LABEL_H
