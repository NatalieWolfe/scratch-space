
#include <cctype>
#include <curl/curl.h>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace std;

class ResponseParser {
public:
    ResponseParser():
        m_state(Status),
        m_status_code(0),
        m_line_read(false)
    {
        // Since we aren't dealing with cookies, vast majority of responses will have headers
        // shorter than this.
        m_line.reserve(256);
    }

    void update(const char* data, const size_t size){
        for (size_t pos = 0; pos < size; ) {
            switch (m_state) {
                case Status:    pos += _update_status(  data + pos, size - pos); break;
                case Headers:   pos += _update_headers( data + pos, size - pos); break;
                case Body:      pos += _update_body(    data + pos, size - pos); break;
            }
        }
    }

    const std::string& status_line() const {
        return m_status_line;
    }

    const std::map<std::string, std::string>& headers() const {
        return m_headers;
    }

    const std::string& body() const {
        return m_body;
    }

private:
    enum State {
        Status,
        Headers,
        Body
    };

    int m_state;
    int m_status_code;
    std::string m_status_line;
    std::map<std::string, std::string> m_headers;
    std::string m_body;

    bool m_line_read;
    std::string m_line;

    int _eol() const {
        // If we're empty, we haven't reached EOL.
        const size_t size = m_line.size();
        if (size == 0) {
            return 0;
        }

        // If we have one character and it is a newline, we've reached EOL. Else, if we have one
        // character and it _isn't_ a newline, we haven't reached EOL.
        const char end_one = m_line.at(size - 1);
        if (size == 1 && end_one == '\n') {
            return 1;
        }
        else if (size == 1) {
            return 0;
        }

        // If we have more than one character and the last two are `\r\n` we've reached a 2-char
        // line ending. Else, if the last character is a `\n` then we've reached a 1-char line
        // ending. Otherwise we have not reached EOL yet.
        const char end_two = m_line.at(size - 2);
        if (end_two == '\r' && end_one == '\n') {
            return 2;
        }
        else if (end_one == '\n') {
            return 1;
        }
        else {
            return 0;
        }
    }

    size_t _read_line(const char* data, const size_t size){
        for (size_t i = 0; i < size; ++i) {
            m_line.push_back(data[i]);

            // If we've reached the end of the line clean it up and respond.
            const int eol = _eol();
            if (eol) {
                m_line_read = true;
                m_line.erase(m_line.size() - eol, eol); // Remove the line ending.
                return i + eol - 1;
            }
        }

        // Ooops, we've reached the end without finding a whole line. Report that we've consumed
        // everything.
        m_line_read = false;
        return size;
    }

    size_t _update_status(const char* data, const size_t size){
        // Read a line.
        const size_t used = _read_line(data, size);

        // If we succeeded in reading a line, copy it to the status line and move to the next state.
        if (m_line_read) {
            ++m_state;
            m_status_line = m_line;
            m_line.erase();
        }

        // Report how many characters we used.
        return used;
    }

    size_t _update_headers(const char* data, const size_t size){
        size_t total_used = 0;
        for (size_t used = 0; m_state == Headers && total_used < size; total_used += used) {
            // Try to read a line.
            used = _read_line(data + total_used, size - total_used);

            // If we didn't read a full line, just move along.
            if (!m_line_read) {
                continue;
            }

            // Is the line empty? We've reached the end of the headers!
            if (m_line.size() == 0) {
                ++m_state;
                continue;
            }

            // We have a header, parse it and clean up the line.
            _parse_header(m_line);
            m_line.erase();
        }

        return total_used;
    }

    void _parse_header(const std::string& line){
        const size_t line_size = line.size();

        // Extract the name.
        size_t name_start = 0;
        size_t name_end = 0;
        size_t colon_pos = 0;
        for (size_t i = 0; i < line_size && isspace(line.at(i)); ++i) {
            name_start = i + 1;
        }
        name_end = name_start;
        for (size_t i = name_start; i < line_size && line.at(i) != ':'; ++i) {
            colon_pos = i + 1;
            if (!isspace(line.at(i))) {
                name_end = i;
            }
        }
        std::string name(line, name_start, name_end - name_start + 1);

        // Make sure the name boundaries make sense. If they don't, just ignore this line.
        if (name_start == line_size || name_start == name_end) {
            return;
        }

        // Extract the value.
        size_t value_start = 0;
        size_t value_end = 0;
        for (size_t i = colon_pos + 1; i < line_size && isspace(line.at(i)); ++i) {
            value_start = i + 1;
        }
        value_end = value_start;
        for (size_t i = value_start; i < line_size; ++i) {
            if (!isspace(line.at(i))) {
                value_end = i;
            }
        }
        std::string value(line, value_start, value_end - value_start + 1);

        m_headers.emplace(std::move(name), std::move(value));
    }

    size_t _update_body(const char* data, const size_t size){
        m_body.append(data, size);
        return size;
    }
};

int main(){
    CURL* handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, "https://en.wikipedia.org/wiki/Main_Page");
    curl_easy_setopt(handle, CURLOPT_HEADER, true);

    curl_slist* req_headers = nullptr;
    req_headers = curl_slist_append(req_headers, "Accept-Encoding: gzip");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, req_headers);

    typedef size_t(*cb)(const char*, const size_t, const size_t, void*);
    ResponseParser parser;
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &parser);
    curl_easy_setopt(
        handle,
        CURLOPT_WRITEFUNCTION,
        (cb)[](const char* buffer, const size_t char_size, const size_t n_chars, void* userp){
            cout << "Chunk: " << char_size << " x " << n_chars << endl;
            ((ResponseParser*)userp)->update(buffer, char_size * n_chars);
            return char_size * n_chars;
        }
    );

    curl_easy_perform(handle);

    cout << "Status: " << parser.status_line() << endl;
    cout << "Headers:" << endl;
    for (auto&& header : parser.headers()) {
        cout << header.first << ": " << header.second << endl;
    }
    cout << "Body length: " << parser.body().size() << endl;

    return 0;
}
