#ifndef SECURESTOREPASS_HPP
#define SECURESTOREPASS_HPP

/* The securestorepass module will handle everything related to storing and retrieveing a password stored in a plaintext on the disk.
 * in ~/.config/mangakon/secure, there should be a file called usr.toml.
 * user.toml includes the following KVPs: user, pass, key, secret.
 * pass and secret should be encrypted in a file found in a file called ~/.config/mangakon/.shadow/secrets
 * secrets contains three values:
 * The hash for the user.toml file,
 * The key for pass,
 * and the key for secret.
 * If the file does not exist, it is generated by spawning an apikeygamestate.
 * if the file does not contain all the keys, it does the same thing with fields autopopulated.
 * if the file does not have the same hash, but all the keys exist, then a new encryption is generated for the keys and values (unless block_encrypt = true)
 * otherwise, we just initialize as usual. Everything should happen in the constructor.
 */

#include <fstream>
#include <filesystem>

#include <tomlplusplus/toml.hpp>

#include "utils.hpp"

#if defined(_WIN32) or defined(_WIN64) or defined(CYGWIN) or defined(_WINNT)
#include <windows.h>
#endif

class SecureStorePass {
public:
    explicit SecureStorePass() {
        generate();
    }

    inline bool generate() {
        m_user.clear();
        m_password.clear();
        m_apikey.clear();
        m_secret.clear();
        m_encrypt = true;

        // TODO: Make cross platform
        namespace fs = std::filesystem;

        fs::path base_path = fs::path(util::dirs::get_home());
        fs::path user_store = base_path;
        fs::path shadow_store = base_path;

        if constexpr(util::platform::UNIX_LIKE) {
            user_store /= fs::path(".config") / fs::path("mangakon") / fs::path("userstore");
            shadow_store /= fs::path(".config") / fs::path("mangakon") / fs::path(".shadow");

        }
        else if constexpr(util::platform::NT_LIKE) {
            user_store /= fs::path("Mangakon") / fs::path("userstore");
            shadow_store /= fs::path("Mangakon") / fs::path(".shadow");
        }

        if (!fs::exists(user_store))
            fs::create_directories(user_store);

        if (!fs::exists(shadow_store)) {
            fs::create_directories(shadow_store);

            #if defined(_WIN32) or defined(_WIN64) or defined(CYGWIN) or defined(_WINNT) // {
                SetFileAttributes(shadow_store.c_str(), FILE_ATTRIBUTE_HIDDEN);
            #endif // }
        }

        fs::path user_file = user_store / "user.toml";
        // Check if user file already exists. get the hash for shadow.
        if (!fs::exists(user_file)) {
            std::ofstream user(user_file);
            user.close();
            return false;
        }

        toml::table user_toml;

        try {
            user_toml = toml::parse_file(user_file.string());
        }
        catch (toml::v3::ex::parse_error &e) {
            return false;
        }

        if (user_toml.contains("user")) {
            m_user = user_toml["user"].value<std::string>().value();
        }
        else {
            return false;
        }

        if (user_toml.contains("password")) {
            m_password = user_toml["password"].value<std::string>().value();
        }
        else {
            return false;
        }

        if (user_toml.contains("apikey")) {
            m_apikey = user_toml["apikey"].value<std::string>().value();
        }
        else {
            return false;
        }

        if (user_toml.contains("secret")) {
            m_secret = user_toml["secret"].value<std::string>().value();
        }
        else {
            return false;
        }

        if (user_toml.contains("encrypt")) {
            m_encrypt = user_toml["encrypt"].value<bool>().value();
        }
        else {
            m_encrypt = true;
        }

        size_t hash = -1;
        std::string str_hash;
        hash = util::hash::hash_file_contents(user_file.string());
        str_hash = std::to_string(hash);



        fs::path shadow_file = shadow_store / str_hash;
        // TODO: save shadow file to the correct updated hash.
        if (m_encrypt and !fs::exists(shadow_file)) {
            std::vector<unsigned char> passkey = util::hash::generate_random_bytes(32);
            std::vector<unsigned char> passiv = util::hash::generate_random_bytes(16);

            m_passkey = passkey;
            m_passiv = passiv;

            std::string passkeystr = util::hash::base64_encode(passkey);
            std::string passivstr = util::hash::base64_encode(passiv);

            std::vector<unsigned char> sctkey = util::hash::generate_random_bytes(32);
            std::vector<unsigned char> sctiv = util::hash::generate_random_bytes(16);

            m_secretkey = sctkey;
            m_secretiv = sctiv;

            std::string sctkeystr = util::hash::base64_encode(sctkey);
            std::string sctivstr = util::hash::base64_encode(passiv);

            std::vector<unsigned char> passcrypt = util::hash::encrypt_string(m_password, passkey, passiv);

            m_password = std::string(passcrypt.begin(), passcrypt.end());

            std::string passcryptstr = util::hash::base64_encode(passcrypt);
            std::vector<unsigned char> sctcrypt = util::hash::encrypt_string(m_secret, sctkey, sctiv);

            m_secret = std::string(sctcrypt.begin(), sctcrypt.end());

            std::string sctcryptstr = util::hash::base64_encode(sctcrypt);

            std::ofstream user_file_out(user_file, std::ios::out | std::ios::trunc);

            std::string enc = m_encrypt ? "" : "\nencrypt = false";
            user_file_out << "user = \"" << m_user << "\"\npassword = \"" << passcryptstr << "\"\napikey = \"" << m_apikey << "\"\nsecret = \"" << sctcryptstr << '\"' << enc;

            user_file_out.close();

            std::ofstream hash_file_out(shadow_file, std::ios::out | std::ios::trunc);

            hash_file_out << passkeystr << '\n' << passivstr << '\n' << sctkeystr << '\n' << sctivstr;

            hash_file_out.close();
        }
        else if (m_encrypt and fs::exists(shadow_file)) {
            std::vector<unsigned char> passdecrypt = util::hash::base64_decode(m_password);
            std::vector<unsigned char> sctdecrypt = util::hash::base64_decode(m_secret);

            m_password = std::string(passdecrypt.begin(), passdecrypt.end());
            m_secret = std::string(sctdecrypt.begin(), sctdecrypt.end());

            std::ifstream file(shadow_file);

            std::string line;

            // Passkey
            std::getline(file, line);
            m_passkey = util::hash::base64_decode(line);

            // Passiv
            std::getline(file, line);
            m_passiv = util::hash::base64_decode(line);

            // Secretkey
            std::getline(file, line);
            m_secretkey = util::hash::base64_decode(line);


            // Secretiv
            std::getline(file, line);
            m_secretiv = util::hash::base64_decode(line);
        }

        return true;
    }

    std::string user() const { return m_user; }
    std::string password() const {

        if (m_encrypt) {
            std::vector<unsigned char> vec(m_password.begin(), m_password.end());
            return util::hash::decrypt_string(vec, m_passkey, m_passiv);
        }

        return m_password;

    }
    std::string apikey() const { return m_apikey; }
    std::string secret() const {
        if (m_encrypt) {
            std::vector<unsigned char> vec(m_secret.begin(), m_secret.end());
            return util::hash::decrypt_string(vec, m_secretkey, m_secretiv);
        }

        return m_secret;
    }

    bool encrypt() const { return m_encrypt; }

    std::wstring get_json() const {
        return L""; // TODO: Fixme
    }

    bool valid() const {
        return !m_user.empty() and !m_password.empty() and !m_apikey.empty() and !m_secret.empty();
    }

private:

    std::string m_user;
    std::string m_password;
    std::string m_apikey;
    std::string m_secret;

    std::vector<unsigned char> m_passkey;
    std::vector<unsigned char> m_passiv;

    std::vector<unsigned char> m_secretkey;
    std::vector<unsigned char> m_secretiv;

    bool m_encrypt;
};

#endif
