

# 🧠 BitBuffer – Compact Bit-Level Data Writing for C++

BitBuffer is a lightweight and efficient C++ class for writing and reading data at the **bit level**, allowing **highly optimized storage** for integers, decimals, booleans, and strings. It’s perfect for serialization, custom file formats, networking, or anywhere you need tight control over size.

## 🚀 Features

- Write data at the **bit level**, not just by bytes.
- Supports `UInt`, `Int`, `bool`, `char`, `string`, and custom `Decimal` and `UDecimal` types.
- Custom charset support for even more compact string encoding.
- Read/write from files directly.
- Bit-accurate reading — read exactly how you wrote.
- Designed with **performance** and **storage savings** in mind.

---

## 🧩 Why Use It?

Because wasting storage is gay.

Writing full `int`s or `float`s when you only need 12 bits or 10 characters? That adds up — fast.

BitBuffer allows you to save **50–60%** on average in data size by letting you choose exactly how many bits to use.

---

## 🔧 Custom STL Replacements

To avoid STL bloat and have full control, BitBuffer uses a set of fast, simple, custom-built tools:

| Header       | Purpose                                                                 |
|--------------|-------------------------------------------------------------------------|
| `list.h`     | Custom dynamic array (like `std::vector`) but leaner and more direct.   |
| `str.h`      | Custom string type (`str`) or alias; used to avoid STL string overhead. |
| `file.h`     | Minimal file I/O helpers: `WriteFileBytes`, `ReadFileBytes`.            |
| `decimal.h`  | `Decimal` and `UDecimal` for storing compact floating-point values.     |

These are designed to give you more control, reduce memory, and reduce hidden allocations and behavior.

---

## 📦 Features

- 🔬 **Bit-level write/read**: `bool`, `UInt`, `Int`, `Decimal`, `char`, `string`, and more.
- 💾 File I/O built-in: read/write buffers directly to/from disk.
- 🧠 Charset compression for strings (4/5/6/7 bits-per-char).
- 💡 Up to **60% storage savings** on real data.
- 📏 Full control of bit widths: you decide how many bits each number uses.

---

## 🛠️ BitBuffer Usage Example

```cpp
BitBuffer buffer;

buffer.WriteUInt(127, 7);        // 7-bit unsigned int
buffer.WriteInt(-10, 6);         // 6-bit signed int
buffer.Write(true);              // boolean (1 bit)
buffer.WriteString("YO!", 6);    // compact string, 6 bits per char

buffer.ResetRead();
int u = buffer.ReadUInt(7);
int i = buffer.ReadInt(6);
bool b = buffer.Read();
str s = buffer.ReadString(6);
```

## 📝 Note
- No idea what I am doing, this is my first C++ project thingy because C# is just shit. Give me some slack

## 📞 Contact Me
- Discord user: ptr.h
- das it
