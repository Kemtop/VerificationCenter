openssl genrsa 2048 > privateKey.pem
openssl pkcs8 -in privateKey.pem -out privateKey -topk8 -nocrypt -outform der
rm privateKey.pem
