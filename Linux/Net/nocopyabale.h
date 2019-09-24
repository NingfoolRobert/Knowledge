#pragma once

class nocopyable
{
private:
	nocopyable(const nocopyable* x) = delete;
	nocopyable& operator=(const nocopyable& x) =delete;
public:
	nocopyable() = default;
	virtual ~nocopyable() = default;
};
