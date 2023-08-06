template<typename Type>
concept Concept = requires(Type t) { Type::Value; };

struct SatisfConcept {
	static constexpr int Value = 42;
};

template<typename...>
struct SomeType {

	template<Concept auto Some>
	static constexpr int Value = Some.Value;

};

static_assert(
	SomeType<>::Value<SatisfConcept{}> == 42
);

int main() {}