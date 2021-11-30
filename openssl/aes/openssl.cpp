#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <inaccel/openssl/aes.h>
#include <inaccel/shm>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/modes.h>
#include <sys/time.h>
#include <unistd.h>

#include <fstream>
#include <regex>

static void cbc_encrypt(const std::pmr::vector<unsigned char> &in, std::pmr::vector<unsigned char> &out, const unsigned char *userKey, const int bits, unsigned char *ivec) {
	AES_KEY key;
	if (AES_set_encrypt_key(userKey, bits, &key)) {
		throw std::runtime_error("AES_set_encrypt_key");
	}

	unsigned char tmp[AES_BLOCK_SIZE];
	memcpy(tmp, ivec, sizeof(tmp));

	struct timeval a, b, res;
	gettimeofday(&b, NULL);

	AES_cbc_encrypt(in.data(), out.data(), in.size(), &key, tmp, AES_ENCRYPT);

	gettimeofday(&a, NULL);
	timersub(&a, &b, &res);
	printf("AES_cbc_encrypt: %ld.%06ld sec\n", res.tv_sec, res.tv_usec);
}

static void golden_cbc_decrypt(const std::pmr::vector<unsigned char> &in, std::pmr::vector<unsigned char> &out, const unsigned char *userKey, const int bits, unsigned char *ivec) {
	AES_KEY key;
	if (AES_set_decrypt_key(userKey, bits, &key)) {
		throw std::runtime_error("AES_set_decrypt_key");
	}

	unsigned char tmp[AES_BLOCK_SIZE];
	memcpy(tmp, ivec, sizeof(tmp));

	struct timeval a, b, res;
	gettimeofday(&b, NULL);

	AES_cbc_encrypt(in.data(), out.data(), in.size(), &key, tmp, AES_DECRYPT);

	gettimeofday(&a, NULL);
	timersub(&a, &b, &res);
	printf("AES_cbc_decrypt: %ld.%06ld sec\n", res.tv_sec, res.tv_usec);
}

static int inaccel_cbc_decrypt(const std::pmr::vector<unsigned char> &in, std::pmr::vector<unsigned char> &out, const unsigned char *userKey, const int bits, unsigned char *ivec) {
	inaccel_AES_KEY key;
	if (inaccel_AES_set_decrypt_key(userKey, bits, &key)) {
		throw std::runtime_error("inaccel_AES_set_decrypt_key");
	}

	unsigned char tmp[AES_BLOCK_SIZE];
	memcpy(tmp, ivec, sizeof(tmp));

	struct timeval a, b, res;
	gettimeofday(&b, NULL);

	if (inaccel_AES_cbc_encrypt(in.data(), out.data(), ((in.size() - 1 ) | (AES_BLOCK_SIZE - 1)) + 1, &key, tmp, AES_DECRYPT)) {
		return -1;
	}

	gettimeofday(&a, NULL);
	timersub(&a, &b, &res);
	printf("inaccel_AES_cbc_decrypt: %ld.%06ld sec\n", res.tv_sec, res.tv_usec);

	return 0;
}

static void cbc_decrypt(const std::pmr::vector<unsigned char> &in, std::pmr::vector<unsigned char> &out, const unsigned char *userKey, const int bits, unsigned char *ivec) {
	if (!in.get_allocator().resource()->is_equal(*inaccel::pmr::resource()) || !out.get_allocator().resource()->is_equal(*inaccel::pmr::resource()) || inaccel_cbc_decrypt(in, out, userKey, bits, ivec)) {
		golden_cbc_decrypt(in, out, userKey, bits, ivec);
	}
}

static void cfb_encrypt(const std::pmr::vector<unsigned char> &in, std::pmr::vector<unsigned char> &out, const unsigned char *userKey, const int bits, unsigned char *ivec) {
	AES_KEY key;
	if (AES_set_encrypt_key(userKey, bits, &key)) {
		throw std::runtime_error("AES_set_encrypt_key");
	}

	unsigned char tmp[AES_BLOCK_SIZE];
	memcpy(tmp, ivec, sizeof(tmp));

	struct timeval a, b, res;
	gettimeofday(&b, NULL);

	int num = 0;
	AES_cfb128_encrypt(in.data(), out.data(), in.size(), &key, tmp, &num, AES_ENCRYPT);

	gettimeofday(&a, NULL);
	timersub(&a, &b, &res);
	printf("AES_cfb128_encrypt: %ld.%06ld sec\n", res.tv_sec, res.tv_usec);
}

static void golden_cfb_decrypt(const std::pmr::vector<unsigned char> &in, std::pmr::vector<unsigned char> &out, const unsigned char *userKey, const int bits, unsigned char *ivec) {
	AES_KEY key;
	if (AES_set_encrypt_key(userKey, bits, &key)) {
		throw std::runtime_error("AES_set_encrypt_key");
	}

	unsigned char tmp[AES_BLOCK_SIZE];
	memcpy(tmp, ivec, sizeof(tmp));

	struct timeval a, b, res;
	gettimeofday(&b, NULL);

	int num = 0;
	AES_cfb128_encrypt(in.data(), out.data(), in.size(), &key, tmp, &num, AES_DECRYPT);

	gettimeofday(&a, NULL);
	timersub(&a, &b, &res);
	printf("AES_cfb128_decrypt: %ld.%06ld sec\n", res.tv_sec, res.tv_usec);
}

static int inaccel_cfb_decrypt(const std::pmr::vector<unsigned char> &in, std::pmr::vector<unsigned char> &out, const unsigned char *userKey, const int bits, unsigned char *ivec) {
	inaccel_AES_KEY key;
	if (inaccel_AES_set_encrypt_key(userKey, bits, &key)) {
		throw std::runtime_error("inaccel_AES_set_encrypt_key");
	}

	unsigned char tmp[AES_BLOCK_SIZE];
	memcpy(tmp, ivec, sizeof(tmp));

	struct timeval a, b, res;
	gettimeofday(&b, NULL);

	int num = 0;
	if (inaccel_AES_cfb128_encrypt(in.data(), out.data(), ((in.size() - 1) | (AES_BLOCK_SIZE - 1)) + 1, &key, tmp, &num, AES_DECRYPT)) {
		return -1;
	}

	gettimeofday(&a, NULL);
	timersub(&a, &b, &res);
	printf("inaccel_AES_cfb128_decrypt: %ld.%06ld sec\n", res.tv_sec, res.tv_usec);

	return 0;
}

static void cfb_decrypt(const std::pmr::vector<unsigned char> &in, std::pmr::vector<unsigned char> &out, const unsigned char *userKey, const int bits, unsigned char *ivec)  {
	if (!in.get_allocator().resource()->is_equal(*inaccel::pmr::resource()) || !out.get_allocator().resource()->is_equal(*inaccel::pmr::resource()) || inaccel_cfb_decrypt(in, out, userKey, bits, ivec)) {
		golden_cfb_decrypt(in, out, userKey, bits, ivec);
	}
}

static void golden_ctr_crypt(const std::pmr::vector<unsigned char> &in, std::pmr::vector<unsigned char> &out, const unsigned char *userKey, const int bits, unsigned char *ivec) {
	AES_KEY key;
	if (AES_set_encrypt_key(userKey, bits, &key)) {
		throw std::runtime_error("AES_set_encrypt_key");
	}

	unsigned char tmp[AES_BLOCK_SIZE];
	memcpy(tmp, ivec, sizeof(tmp));

	struct timeval a, b, res;
	gettimeofday(&b, NULL);

	unsigned char ecount_buf[AES_BLOCK_SIZE];
	unsigned int num = 0;
	CRYPTO_ctr128_encrypt(in.data(), out.data(), in.size(), &key, tmp, (unsigned char *) &ecount_buf, &num, (block128_f) AES_encrypt);

	gettimeofday(&a, NULL);
	timersub(&a, &b, &res);
	printf("AES_ctr128_crypt: %ld.%06ld sec\n", res.tv_sec, res.tv_usec);
}

static int inaccel_ctr_crypt(const std::pmr::vector<unsigned char> &in, std::pmr::vector<unsigned char> &out, const unsigned char *userKey, const int bits, unsigned char *ivec) {
	inaccel_AES_KEY key;
	if (inaccel_AES_set_encrypt_key(userKey, bits, &key)) {
		throw std::runtime_error("AES_set_encrypt_key");
	}

	unsigned char tmp[AES_BLOCK_SIZE];
	memcpy(tmp, ivec, sizeof(tmp));

	struct timeval a, b, res;
	gettimeofday(&b, NULL);

	unsigned char ecount_buf[AES_BLOCK_SIZE];
	unsigned int num = 0;
	if (inaccel_AES_ctr128_encrypt(in.data(), out.data(), ((in.size() - 1) | (AES_BLOCK_SIZE - 1)) + 1, &key, tmp, (unsigned char *) &ecount_buf, &num)) {
		return -1;
	}

	gettimeofday(&a, NULL);
	timersub(&a, &b, &res);
	printf("inaccel_AES_ctr128_crypt: %ld.%06ld sec\n", res.tv_sec, res.tv_usec);

	return 0;
}

static void ctr_crypt(const std::pmr::vector<unsigned char> &in, std::pmr::vector<unsigned char> &out, const unsigned char *userKey, const int bits, unsigned char *ivec) {
	if (!in.get_allocator().resource()->is_equal(*inaccel::pmr::resource()) || !out.get_allocator().resource()->is_equal(*inaccel::pmr::resource()) || inaccel_ctr_crypt(in, out, userKey, bits, ivec)) {
		golden_ctr_crypt(in, out, userKey, bits, ivec);
	}
}

class Object {

public:

	virtual void read(std::pmr::vector<unsigned char> &vector) = 0;

	virtual void write(const std::pmr::vector<unsigned char> &vector) = 0;

	virtual ~Object() {}

};

class File: public Object {

private:

	char *path;

public:

	File(const char *uri) {
		std::string tmp(uri);
		std::smatch m;
		if (std::regex_search(tmp, m, std::regex("file:(?://)?(.+)"))) {
			this->path = strdup(m[1].str().c_str());
		} else {
			throw std::invalid_argument(uri);
		}
	}

	void read(std::pmr::vector<unsigned char> &vector) {
		std::ifstream is;
		is.open(this->path, std::ifstream::in);
		is.seekg (0, is.end);
		vector.resize(is.tellg());
		is.seekg (0, is.beg);
		is.read((char *) vector.data(), vector.size());
		is.close();
	}

	void write(const std::pmr::vector<unsigned char> &vector) {
		std::ofstream os;
		os.open(this->path, std::ofstream::out);
		os.write((char *) vector.data(), vector.size());
		os.close();
	}

	~File() {
		free(path);
	}

};

class S3: public Object {

private:

	char *bucket;
	char *key;

public:

	S3(const char *uri) {
		std::string tmp(uri);
		std::smatch m;
		if (std::regex_search(tmp, m, std::regex("s3://([^/]+)/(.+)"))) {
			bucket = strdup(m[1].str().c_str());
			key = strdup(m[2].str().c_str());
		} else {
			throw std::invalid_argument(uri);
		}
	}

	void read(std::pmr::vector<unsigned char> &vector) {
		Aws::SDKOptions options;
		Aws::InitAPI(options);

		Aws::Client::ClientConfiguration client_configuration;
		client_configuration.requestTimeoutMs = LONG_MAX;

		Aws::S3::S3Client s3_client(client_configuration);

		Aws::S3::Model::GetObjectRequest get_object_request;
		get_object_request.SetBucket(bucket);
		get_object_request.SetKey(key);

		Aws::S3::Model::GetObjectOutcome get_object_outcome = s3_client.GetObject(get_object_request);
		if (!get_object_outcome.IsSuccess()) {
			Aws::ShutdownAPI(options);

			throw std::runtime_error(get_object_outcome.GetError().GetMessage());
		}

		Aws::IOStream &is = get_object_outcome.GetResult().GetBody();
		is.seekg (0, is.end);
		vector.resize(is.tellg());
		is.seekg (0, is.beg);
		is.read((char *) vector.data(), vector.size());

		Aws::ShutdownAPI(options);
	}

	void write(const std::pmr::vector<unsigned char> &vector) {
		Aws::SDKOptions options;
		Aws::InitAPI(options);

		Aws::Client::ClientConfiguration client_configuration;
		client_configuration.requestTimeoutMs = LONG_MAX;

		Aws::S3::S3Client s3_client(client_configuration);

		std::shared_ptr<Aws::IOStream> os = Aws::MakeShared<Aws::StringStream>("PutObjectInputStream", std::stringstream::binary | std::stringstream::in | std::stringstream::out);
		os->write((char *) vector.data(), vector.size());

		Aws::S3::Model::PutObjectRequest put_object_request;
		put_object_request.SetBody(os);
		put_object_request.SetBucket(bucket);
		put_object_request.SetKey(key);

		Aws::S3::Model::PutObjectOutcome put_object_outcome = s3_client.PutObject(put_object_request);
		if (!put_object_outcome.IsSuccess()) {
			Aws::ShutdownAPI(options);

			throw std::runtime_error(put_object_outcome.GetError().GetMessage());
		}

		Aws::ShutdownAPI(options);
	}

	~S3() {
		free(bucket);
		free(key);
	}

};

class Data {

public:

	Object *in, *out;

	Data(const char *in, const char *out) {
		try {
			this->in = new File(in);
		} catch(std::invalid_argument &_) {
			this->in = new S3(in);
		}

		try {
			this->out = new File(out);
		} catch(std::invalid_argument &_) {
			this->out = new S3(out);
		}
	}

	~Data() {
		delete in;
		delete out;
	}

};

int main(int argc, char *argv[]) {
	int enc = 1;

	int opt;
	while ((opt = getopt(argc, argv, "de")) != -1) {
		switch (opt) {
		case 'd':
			enc = 0;
			break;
		case 'e':
			enc = 1;
			break;
		}
	}

	unsigned char userKey[EVP_MAX_KEY_LENGTH];
	unsigned char ivec[EVP_MAX_IV_LENGTH];
	unsigned char userKey_ivec[sizeof(userKey) + sizeof(ivec)];

	char *cipher = getenv("cipher");
	if (!cipher) {
		return EXIT_SUCCESS;
	}

	const EVP_CIPHER *e = EVP_get_cipherbyname(cipher);
	if (!e) {
		fprintf(stderr, "%s is not a known cipher\n", cipher);
		return EXIT_FAILURE;
	}

	char *pass = getenv("pass");
	if (!pass || !strlen(pass)) {
		pass = getpass("enter password: ");
	}

	if (!PKCS5_PBKDF2_HMAC(pass, strlen(pass), NULL, 0, 10000, EVP_sha256(), EVP_CIPHER_key_length(e) + EVP_CIPHER_iv_length(e), userKey_ivec)) {
		fprintf(stderr, "PKCS5_PBKDF2_HMAC failed\n");
		return EXIT_FAILURE;
	}

	memcpy(userKey, userKey_ivec, EVP_CIPHER_key_length(e));
	memcpy(ivec, userKey_ivec + EVP_CIPHER_key_length(e), EVP_CIPHER_iv_length(e));

	Data data(getenv("in"), getenv("out"));

	std::pmr::vector<unsigned char> *in;
	try {
		in = new std::pmr::vector<unsigned char>(inaccel::pmr::resource());

		data.in->read(*in);
	} catch (...) {
		in = new std::pmr::vector<unsigned char>();

		data.in->read(*in);
	}

	std::pmr::vector<unsigned char> *out;
	try {
		out = new std::pmr::vector<unsigned char>(in->size(), inaccel::pmr::resource());
	} catch (...) {
		out = new std::pmr::vector<unsigned char>(in->size());
	}

	switch (EVP_CIPHER_mode(e)) {
	case EVP_CIPH_CBC_MODE:
		if (enc) {
			// PKCS#7 padding
			unsigned char padding = EVP_CIPHER_block_size(e) - in->size() % EVP_CIPHER_block_size(e);
			in->resize(in->size() + padding, padding);
			out->resize(out->size() + padding);

			cbc_encrypt(*in, *out, userKey, EVP_CIPHER_key_length(e) * CHAR_BIT, ivec);
		} else {
			cbc_decrypt(*in, *out, userKey, EVP_CIPHER_key_length(e) * CHAR_BIT, ivec);

			// PKCS#7 padding
			unsigned char padding = out->back();
			out->resize(out->size() - padding);
		}
		break;
	case EVP_CIPH_CFB_MODE:
		if (enc) {
			cfb_encrypt(*in, *out, userKey, EVP_CIPHER_key_length(e) * CHAR_BIT, ivec);
		} else {
			cfb_decrypt(*in, *out, userKey, EVP_CIPHER_key_length(e) * CHAR_BIT, ivec);
		}
		break;
	case EVP_CIPH_CTR_MODE:
		ctr_crypt(*in, *out, userKey, EVP_CIPHER_key_length(e) * CHAR_BIT, ivec);
		break;
	default:
		fprintf(stderr, "%s cipher not supported\n", cipher);
		return EXIT_FAILURE;
	}

	data.out->write(*out);

	delete in;
	delete out;
}
