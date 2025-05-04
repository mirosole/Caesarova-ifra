
# 🔐 Caesar Cipher Decoder

## 📌 Description

This project is a **decoder for messages encrypted with the Caesar cipher**.
The program uses **brute-force for all possible shifts** and selects the best match based on a "heard" version of the text.

---

## 📂 Project Structure

📁 **File:**

* **`main.c`** — the main file containing the decoding logic.

---

## 🎯 Key Features

✅ **Decodes Caesar cipher text** by trying all possible shift values
✅ **Finds the optimal shift** by comparing against a "heard" (possibly misspelled) version
✅ **Uses dynamic memory allocation** (`malloc`, `realloc`) for texts of arbitrary length
✅ **Handles input errors**, including invalid characters and differing input lengths

---

## 🛠 Usage

### **🔧 Build the project**

```sh
gcc -o caesar main.c
```

### **▶️ Run the program**

The program takes **two input strings**:
1️⃣ **Encrypted text**
2️⃣ **Heard text** (possibly with small errors)

#### **Example**

```sh
$ ./caesar
xUbbemehbT
XYlloworld
```

Output:

```
Helloworld
```

---

## ⚙️ Optional Mode

Use the **`-prp-optional`** flag to allow **inputs of different lengths**:

```sh
./caesar -prp-optional
```

---

## ⚠️ Error Handling

| Error                                   | Message to stderr             | Exit Code |
| --------------------------------------- | ----------------------------- | --------- |
| Invalid characters in input             | `Error: Chybny vstup!`        | 100       |
| Different string lengths (default mode) | `Error: Chybna delka vstupu!` | 101       |

---

## 📋 Requirements

📌 **C99 or later**
📌 **GCC compiler**
📌 **Linux/Debian OS** (for compatibility with the grading system)

---

## 🚀 Development

👨‍💻 **Oleksii Miroshnichenko**
📅 Developed as part of a university assignment.

---

