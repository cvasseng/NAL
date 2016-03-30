***This is my old C++ UDP networking library. It was originally written around 2007 (last changed in 2008 or 2009 I think), and is added here mostly as a "sentimental backup".***

# NAL

NAL (Network Abstraction Library - creative, huh?) is a networking library built on top of ENET (though it was written to make it easy to switch ENET out for something else if needed). It features things like an easy-to-use packet serialization system, and packet queues. It's a pretty simple and thin library. I used it for several projects up until 2010 or so. 

There's probably a few issues with this code (I'm guessing there are memory leaks for instance) - it was mostly used as a quick and dirty way of getting networking up and running in my various experiments. It may contain some half useful code, like the serialization stuff.

# LICENSE

MIT. See [LICENSE](LICENSE).