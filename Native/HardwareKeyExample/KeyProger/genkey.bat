openssl genrsa 2048 > privkey.pem
openssl pkcs8 -in privkey.pem -out prkey -topk8 -nocrypt -outform der
rm privkey.pem
