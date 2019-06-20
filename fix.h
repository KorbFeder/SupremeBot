#ifndef OPENSSLFIX_H
#define OPENSSLFIX_H

#include <QSslConfiguration>

class OpenSslFix {
public:
    static void fixCaCertificates()
    {
        QSslConfiguration config(QSslConfiguration::defaultConfiguration());
        QList<QSslCertificate> in(config.caCertificates());
        QList<QSslCertificate> out;

        for (int i=0, size=in.size(); i<size; ++i) {
            const QSslCertificate &c(in[i]);
            if (!c.isBlacklisted()) {
                /* not expired -> add */
                out << c;
                continue;
            }

            /* check if the cert is already present in the output */
            bool found = false;
            for (int j=0, size=out.size(); j<size; ++j) {
                if (isCertificateSameName(c, out[j])) {
                    /* already present... */
                    found = true;
                    break;
                }
            }

            if (!found)
                out << c;
        }

        /* now set the new list as the default */
        config.setCaCertificates(out);
        QSslConfiguration::setDefaultConfiguration(config);
    }

private:
    static inline bool isCertificateSameName(const QSslCertificate &cert1,
                                             const QSslCertificate &cert2)
    {
        return cert1.subjectInfo(QSslCertificate::Organization) ==
                cert2.subjectInfo(QSslCertificate::Organization) &&
                cert1.subjectInfo(QSslCertificate::CommonName) ==
                cert2.subjectInfo(QSslCertificate::CommonName) &&
                cert1.subjectInfo(QSslCertificate::LocalityName) ==
                cert2.subjectInfo(QSslCertificate::LocalityName) &&
                cert1.subjectInfo(QSslCertificate::OrganizationalUnitName) ==
                cert2.subjectInfo(QSslCertificate::OrganizationalUnitName) &&
                cert1.subjectInfo(QSslCertificate::StateOrProvinceName) ==
                cert2.subjectInfo(QSslCertificate::StateOrProvinceName) &&
                cert1.subjectInfo(QSslCertificate::CountryName) ==
                cert2.subjectInfo(QSslCertificate::CountryName);
    }
};

#endif // OPENSSLFIX_H
