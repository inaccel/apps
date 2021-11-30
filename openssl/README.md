## openssl

```sh
# openssl enc -in <in> -out <out>.encrypted -pass pass:<pass> -nosalt -pbkdf2 -<any-supported-cipher>
docker inaccel -e PWD -e in=file:<in> -e out=file:<out>.encrypted -e pass=<pass> -p inaccel/apps -t openssl run <any-supported-cipher>

# openssl enc -in <in>.encrypted -out <out>.decrypted -pass pass:<pass> -d -nosalt -pbkdf2 -<any-supported-cipher>
docker inaccel -e PWD -e in=file:<in>.encrypted -e out=file:<out>.decrypted -e pass=<pass> -p inaccel/apps -t openssl run <any-supported-cipher> -- -d
```
