#!/bin/sh

# Create a PKCS#12 key/certificate archive for signing.
# This is needed for testing the build process as a whole,
# if no 'real' key is available.

set -e

openssl genrsa -des3 -passout pass:1234 -out dubious.key 2048

openssl req -new -key dubious.key -out dubious.csr \
	-passin pass:1234 -passout pass:1234 <<-"EOF"
ZZ

City
Dubious Corp.
Suspicous Fish
UNTRUSTWORTHY.example.com
DONTTRUSTME@example.com


EOF

cat > openssl.cnf <<-"EOF"
[v3_usr]
basicConstraints       = CA:FALSE
subjectKeyIdentifier   = hash
authorityKeyIdentifier = keyid:always, issuer:always
keyUsage               = nonRepudiation, digitalSignature, keyEncipherment
extendedKeyUsage       = codeSigning
nsCertType             = objsign
EOF

openssl x509 -req -extfile openssl.cnf -extensions v3_usr -days 365 \
	-signkey dubious.key -in dubious.csr -out dubious.crt -passin pass:1234

openssl pkcs12 -export -in dubious.crt -inkey dubious.key -out dubious.pfx \
	-passin pass:1234 -passout pass:1234

fp=$(openssl x509 -in dubious.crt -noout -fingerprint | sed 's/.*=//g; s/://g')

echo -e "\033[36mfingerprint:\033[m \033[1;37m$fp\033[m"

rm dubious.key dubious.crt dubious.csr openssl.cnf
