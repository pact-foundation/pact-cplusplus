// consumer.h - Contains declarations of Pact C++ consumer DSL functions
#pragma once

#ifdef PACT_CONSUMER_EXPORTS
#define PACT_CONSUMER_API __declspec(dllexport)
#else
#define PACT_CONSUMER_API __declspec(dllimport)
#endif
