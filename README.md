# KestrelPack
[![Build Status](https://jenkins.shodan.fyi/buildStatus/icon?job=KestrelPack%2Fmaster)](https://jenkins.shodan.fyi/job/KestrelPack/job/master/)
This is a small Compression Library.

## Usage

To Encode and Decode data using Arithmetic Coding the following Steps have to be done:
1. Create a FrequencyModel, that holds (explicitly) one of each Character that has to be encoded.
    This can be done by passing a span to a FrequencyModdel class during initialization.
2. An Encoder or Decoder has to be created
3. Data has to be De- or Encoded using those

`using namespace KestrelPack` is assumed for these examples

### Creating a Frequency Model

To create a Frequency Model pass a list of all Characters to it that can be encountered.
```c++
#include "FrequencyModel.hpp"

// this can be a vector or string, and characters can be duplicated,
// the model will store only unique characters in the constructor
std::vector<char> symbols("ABCD");
FrequencyModel model(syblols);
```

### Creating an Encoder

To create an Encoder, pass the FrequencyModel you just made to it's constructor
```c++
#include "Encoder.hpp"

Encoder encoder(model);
```
The Encoder should now be ready to encode.

### Encoding using the Encoder class

To encode a stream of symbols with the encoder, pass the symbols one by one into
the `encodeSymbol` method.
To finalize the encoding call the `finalizeEncoding` method.
```c++
std::string toEncode = "AAAABBDDCCAABCD";

// create an output stream that will hold the encoded data
std::vector<uint8_t> output;

for(auto symbol : toEncode){
    encoder.encodeSymbol(symbol, output);
}
encoder.finalizeEncoding(output);
```
`output` should now hold the encoded message
The Frequency Model will also be reset by finalizeEncoding. 