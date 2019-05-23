# Mod-Polynomial Library

First Created: May 23rd, 2019
Last Updated: May 23rd, 2019

Welcome to library of "mod-polynomial"! This library consists of four files:  
* fast_modint.h
* polynomial_ntt.h
* polynomial_mod.h
* polynomial_mod_in_one.h

Here, `polynomial_mod_in_one.h` is practically the concatenation of three other files. We can use it when you are going to submit to a single-file-submission-format online judges of programming contest websites.

The implementation here is for special 32-bit modulos. If you want to use for special 64-bit modulos, you just have to change definition of `singlebit` and `doublebit` in `fast_modint.h` (or `polynomial_mod_in_one.h`), to `uint64_t` and `__uint128_t`, respectively.

## fast_modint.h
This is the modint-class (class structure of integer of a fixed modulo), which is faster than usual implemetation.
We use [Montgomery modular multiplication](https://en.wikipedia.org/wiki/Montgomery_modular_multiplication) for multiplication.

## polynomial_ntt.h
In this file, we will convolve two polynomials. More specifically, letting A = a<sub>0</sub>x<sup>0</sup> + a<sub>1</sub>x<sup>1</sup> + ... + a<sub>n-1</sub>x<sup>n-1</sup> and B = b<sub>0</sub>x<sup>0</sup> + b<sub>1</sub>x<sup>1</sup> + ... + b<sub>n-1</sub>x<sup>n-1</sup>, `convolve(a, b)` will calculate A×B in O(n log n) time and O(n) memory complexity.

The expected running time is as follows (tested in "custom test" in [atcoder.jp](atcoder.jp)):

|      n      | 2<sup>17</sup> | 2<sup>18</sup> | 2<sup>19</sup> | 2<sup>20</sup> | 2<sup>21</sup> | 2<sup>22</sup> | 2<sup>23</sup> |
|:-----------:|:--------------:|:--------------:|:--------------:|:--------------:|:--------------:|:--------------:|:--------------:|
| time (sec.) |     0.0638     |     0.1403     |     0.2863     |     0.6219     |     1.4022     |     3.2095     |    > 10.0000   |

Multiplication will work if (degree of polynomial) < 2<sup>depth</sup>

## polynomial_mod.h
We will process addition, subtraction, multiplication, division (+ getting remainder) of polynomial in modulo field.
Let n the degree of polynomial. Addition and subtraction can be done in O(n) time complexity, and multiplcation and division can be done in O(n log n) time complexity.
