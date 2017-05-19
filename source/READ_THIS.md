# About JMX

JMX is a byte-byte JSON parser that converts utf8 characters from text form to an equivalent C struct. The design of JMX was motivated by the need for a memory lightweight but usable solution for C/CPP based programs.

The parser has a simple state machine `jmx_state_track` that tracks the progress of the packet that is currently digested, one should use the return code issued by this function to act appropriately.

An example application exists in `main.cpp`. The normal application of JMX is to integrate it into a Serial class that handle communication across the wire such that when a character is received, JMX handles the character before it reaches the user application. Here, it is possible to filter out JMX packets from standard serial communications.

JMX packets are wrapped using slip characters, an example string is shown below:

`"\xdb""{\"status\":{\"code\":999}}""\xc0"`.

# How it works

To receive a packet via JMX, an application developer will have to specify the json structure that they wish to receive. This is called a `JMXActionItem`:

```
JMXActionTable initMap = {
    STANDARD_ACTION,
    "jmx",                        //Packet identifier
    sizeof(JMXInitPacket),        // struct size
    (void **)&jmx_init_p,        // pointer base
    nop,                // result function pointer
    {
    //    KEY            TOKEN TYPE                STORAGE OFFSET INTO STRUCT                    SIZE OF STORAGE BUFFER
    { "enable",        T_STATE_NUMBER,            offsetof(JMXInitPacket, enable),            member_size(JMXInitPacket, enable)    },
    { "v",            T_STATE_STRING,            offsetof(JMXInitPacket, v),                    member_size(JMXInitPacket, v)        },
    }
};
```

An action item is a C struct which stores the packet identifier, the size of the C struct to map it to, a pointer which stores the result, an action function for when a packet is received, and a sequence of keys and types that define the structure.

The an example of the above action item in json form is:

```
{
    "jmx" : {
        "enable": 0,
        "v":"XX.XX.XX"
    }
}
```

This would then be translated to the following C struct at runtime:

```
typedef struct JMXInitPacket_t
{
    int enable;
    char v[9];
} JMXInitPacket;
```

Here we can see that there is a strict limit in place on the "v" field, jmx will abide by this limit.

On reception of a JMXInit packet, the function nop will be invoked with a pointer to the struct in memory.


JSON structures using JMX are limited to a depth of one and must take the form of:

```
{
    "identifier":{
        "key":"value"
    }
}
```

An JMXActionItem is placed into a JMXActionTable that is looked up when a SLIP escape sequence is seen. If action item matches, characters will be digested regardless. If malformed JSON is detected, jmx will reset its state.

Any further questions: j.devine@lancaster.ac.uk 
