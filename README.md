# Pistache

Pistache is a C++ templated string formatter.

This library allow fast string formatting when a "base" string is used with some parameters.

```cpp
#include "pistache.h"

// Base string template
pistache::String str("Hello {name} !");

// Create a context object
pistache::Context ctx;
ctx["name"] = "world";

std::cout << str(ctx) << std::endl;
// output : Hello world !
```

Internally, pistache parse only one time the templated string which allow fast formatting;

```cpp
std::vector<User> very_long_array_of_users;

// Standard way for printing infos
for(const auto& user : very_long_array_of_users)
    std::cout << "User n°" << user.id << "\n"
             << "Name : " << user.name << " "  << user.first_name  << "\n"
             << "Age : " << user.age << "\n"
             << "City : " << user.city << "\n";
```


This approach work only if you format your string at compile time and (according to me) it's a little bit "angle bracket hell".

The same thing with pistache :

```cpp
std::vector<User> very_long_array_of_users;
pistache::String infos("User n°{id}\n"
                       "Name : {name} {first_name}\n"
                       "Age : {age}\n"
                       "City : {city}\n"
                      );

//Create it before loop to construct it only one time                      
pistache::Context ctx;
for(const auto& user : very_long_array_of_users) {
    ctx["id"] = user.id;
    ctx["name"] = user.name;
    ctx["first_name"] = user.first_name;
    ctx["age"] = user.age;
    ctx["city"] = user.city;

    std::cout << info(ctx) << std::endl;
}
```

This approach allow a better reading and we can change formatting at runtime.


# License

This library is provided under the MIT License
