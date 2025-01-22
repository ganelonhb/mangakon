#ifndef UTILS_HPP
#define UTILS_HPP

#define forever while(true)

#include <iostream> // use sparsely
#include <fstream>
#include <string>
#include <codecvt>
#include <locale>
#include <functional>
#include <unistd.h>
#include <termios.h>
#include <filesystem>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

namespace util {
    inline std::wstring str2wstr(const std::string &str) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes(str);
    }

    namespace hash {
        inline size_t hash_file_contents(const std::string &filePath) {
            std::ifstream file(filePath, std::ios::binary);

            if (!file)
                throw std::runtime_error("Failed to open file: " + filePath);

            std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

            std::hash<std::string> hasher;

            return hasher(contents);
        }

        inline std::vector<unsigned char> generate_random_bytes(int numBytes) {
            std::vector<unsigned char> buffer(numBytes);

            if (!RAND_bytes(buffer.data(), numBytes)) {
                throw std::runtime_error("Error generating random bytes.");
            }

            return buffer;
        }

        inline std::vector<unsigned char> encrypt_string(const std::string &plaintext, const std::vector<unsigned char> &key, const std::vector<unsigned char> &iv) {
            EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
            if (!ctx) throw std::runtime_error("Failed to create EVP_CIPHER_CTX");

            if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()) != 1) {
                EVP_CIPHER_CTX_free(ctx);
                throw std::runtime_error("EVP_EncryptInit_ex failed");
            }

            std::vector<unsigned char> ciphertext(plaintext.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
            int len = 0, ciphertext_len = 0;

            if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len,
                                reinterpret_cast<const unsigned char*>(plaintext.data()), plaintext.size()) != 1) {
                EVP_CIPHER_CTX_free(ctx);
                throw std::runtime_error("EVP_EncryptUpdate failed");
            }
            ciphertext_len = len;

            if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
                EVP_CIPHER_CTX_free(ctx);
                throw std::runtime_error("EVP_EncryptFinal_ex failed");
            }
            ciphertext_len += len;

            ciphertext.resize(ciphertext_len);
            EVP_CIPHER_CTX_free(ctx);
            return ciphertext;
        }

        inline std::string decrypt_string(const std::vector<unsigned char> &ciphertext, const std::vector<unsigned char> &key, const std::vector<unsigned char> &iv) {
            EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
            if (!ctx) throw std::runtime_error("Failed to create EVP_CIPHER_CTX");

            if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()) != 1) {
                EVP_CIPHER_CTX_free(ctx);
                throw std::runtime_error("EVP_DecryptInit_ex failed");
            }

            std::vector<unsigned char> plaintext(ciphertext.size());
            int len = 0, plaintext_len = 0;

            if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()) != 1) {
                EVP_CIPHER_CTX_free(ctx);
                throw std::runtime_error("EVP_DecryptUpdate failed");
            }
            plaintext_len = len;

            if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) {
                EVP_CIPHER_CTX_free(ctx);
                throw std::runtime_error("EVP_DecryptFinal_ex failed");
            }
            plaintext_len += len;

            plaintext.resize(plaintext_len);
            EVP_CIPHER_CTX_free(ctx);
            return std::string(plaintext.begin(), plaintext.end());
        }

        inline std::string base64_encode(const std::vector<unsigned char> &data) {
            BIO *bio = BIO_new(BIO_s_mem());

            BIO *b64 = BIO_new(BIO_f_base64());

            BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

            BIO_push(b64, bio);

            BIO_write(b64, data.data(), data.size());
            BIO_flush(b64);

            BUF_MEM *buffer;
            BIO_get_mem_ptr(b64, &buffer);

            std::string encoded(buffer->data, buffer->length);
            BIO_free_all(b64);
            return encoded;
        }

        inline std::vector<unsigned char> base64_decode(const std::string &encoded) {
            BIO *bio = BIO_new_mem_buf(encoded.data(), encoded.size());
            BIO *b64 = BIO_new(BIO_f_base64());

            BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
            BIO_push(b64, bio);

            std::vector<unsigned char> decoded(encoded.size());

            int decoded_len = BIO_read(b64, decoded.data(), decoded.size());

            if (decoded_len < 0) {
                BIO_free_all(b64);
                throw std::runtime_error("Failed to decode base64.");
            }

            decoded.resize(decoded_len);
            BIO_free_all(b64);
            return decoded;
        }
    }

    namespace platform {
        constexpr uint16_t PLATFORM_LINUX = 0;
        constexpr uint16_t PLATFORM_WINDOWS = 1;
        constexpr uint16_t PLATFORM_MACOS = 2;
        constexpr uint16_t PLATFORM_UNIX = 3;
        constexpr uint16_t PLATFORM_BSD = 4;
        constexpr uint16_t PLATFORM_UNKNOWN = 65535;

        constexpr uint16_t get_platform() {
            #if defined(__linux__) or defined(__gnu_linux__)
                return PLATFORM_LINUX;
            #elif defined(_WIN32) or defined(_WIN64) or defined(CYGWIN) or defined(_WINNT)
                return PLATFORM_WINDOWS;
            #elif defined(__APPLE__) or defined(__MACH__) or defined(__DARWIN__)
                return PLATFORM_MACOS;
            #elif defined(__FreeBSD__) or defined(__NetBSD__) or defined(__OpenBSD__) or defined(__DragonFly__) or defined(__MirBSD__)
                return PLATFORM_BSD;
            #elif defind(__unix__)
                return PLATFORM_UNIX;
            #else
                return PLATFORM_UNKNOWN;
            #endif
        }

        constexpr uint16_t PLATFORM = get_platform();
        constexpr bool UNIX_LIKE = PLATFORM == 0 or PLATFORM == 2 or PLATFORM == 3 or PLATFORM == 4;
        constexpr bool NT_LIKE = PLATFORM == 1;
        constexpr bool OSX_LIKE = PLATFORM == 2;
    }

    namespace dirs {
        inline std::string get_home() {
            using namespace util::platform;
            if constexpr(get_platform() == PLATFORM_LINUX or get_platform() == PLATFORM_UNIX or get_platform() == PLATFORM_MACOS) {
                return std::string(std::getenv("HOME"));
            }
            else if constexpr(get_platform() == PLATFORM_WINDOWS) {
                return std::string(std::getenv("APPDATA"));
            }
            else {
                throw std::runtime_error("Unknown or unsupported platform.");
            }
        }
    }

    namespace compile_time_id {
        constexpr size_t cstrlen(const char *str) {
            const char *start = str;
            const char *ptr = str;
            while (*ptr != '\0')
                ++ptr;

            return ptr - start;
        }

        constexpr uint32_t hash_string(const char* str) {
            uint32_t hash = 0x811C9DC5u;

            for (size_t i = 0; i < cstrlen(str); ++i) {
                hash ^= static_cast<uint32_t>(str[i]);
                hash *= 0x1000193u;
            }

            return hash;
        }

        constexpr uint32_t lcg(uint32_t seed) {
            return (0x19660D * seed + 0x3C6EF35F) % 0xFFFFFFFF;
        }

        constexpr int get_id(const char* str) {
            uint32_t seed = hash_string(str);

            return lcg(seed);
        }
    }

    namespace cwstr {
        constexpr int cwcslen(const wchar_t * str) {
            const wchar_t *start = str;
            const wchar_t *ptr = str;
            while (*ptr != L'\0')
            {
                ++ptr;
            }
            return ptr - start;
        }

        constexpr int cwsalen(const wchar_t * const*wstra) {
            int i = 0;

            for (; wstra[i] != nullptr; ++i);

            return i;
        }

        constexpr int cmaxwsalen(const wchar_t* const* wstra) {
            int max = -1;

            for (int i = 0; wstra[i] != nullptr; ++i)
            {
                int len = cwcslen(wstra[i]);
                if (len > max) max = len;
            }

            return max;
        }

    }

    namespace constants {
        namespace align {
            constexpr int CENTER = -1;
            constexpr int LEFT = -2;
            constexpr int RIGHT = -3;
        }
    }

    namespace colors {
        namespace defaults {
            constexpr int SELECTED = 0xE68A00;
            constexpr int SELECTED_UNFOCUSED = 0xE6B873;
            constexpr int WINDOW_BG = 0xFF5E00;
            constexpr int BUTTON = 0xFF7E33;
            constexpr int BUTTON_UNFOCUSED = 0xFFBE99;

            constexpr int TABBAR_BG = 0xE59866;
            constexpr int TABBAR_BG_UNFOCUSED = 0x3E372D;
            constexpr int TABBAR_TAB = 0xCB741D;
            constexpr int TABBAR_TAB_UNFOCUSED = 0xFFDFCD;

            constexpr int BUTTONPRESS = 0xE65400;
        }
    }

    namespace magic {
        inline void cls() {
            std::cout << "\033[2J\033[H" << std::flush;
        }
    }
}

#endif
