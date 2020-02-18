#ifndef CLUB_H
#define CLUB_H
#include <QString>

class club
{

private:
    int m_idClub;
    QString m_name;
    QString m_codi;
    QString m_color;
    int m_propietario;


public:

    ///GETTERS
    QString getName();
    int getClubId();
    QString getCodi();
    QString getColor();
    int getIdPropietari();

    ///SETTERS
    void setName(QString name);
    void setClubId(int idClub);
    void setCodi(QString codi);
    void setColor(QString color);
    void setIdPropietario(int getIdPropietario);


    ///BBDD Connectors
    bool save();
    void load(int id);
};

#endif // CLUB_H
