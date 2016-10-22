// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

/*****************************************************************************/

#include "recipe.hpp"
#include "messages.hpp"

#include "testslib.hpp"

#include <unordered_set>

/*****************************************************************************/

/*
	Test plan:
	Done	1) Basic
	Done		1.1) Simple recipe
	Done		1.2) Several recipes
	Done		1.3) Several recipes with the same name
	Done		1.4) Create recipe with empty name
	Done		1.5) Create recipe with empty description
	Done		1.6) Create recipe with empty author name
	Done	2) Ingredient
	Done		2.1) Add ingredient
	Done		2.2) Add several ingredients
	Done		2.3) Add several ingredients with the same name
	Done		2.4) Add ingredient with empty name
	Done		2.5) Add ingredient with zero value
	Done		2.6) Add ingredient with negative value
	Done		2.7) Has ingredient
	Done		2.8) Has ingredient by empty name
	Done		2.9) Has ingredient by missing name
	Done		2.10) Modify ingredient
	Done		2.11) Modify several ingredients
	Done		2.12) Modify ingredient several times
	Done		2.13) Modify ingredient with empty name
	Done		2.14) Modify ingredient with zero value
	Done		2.15) Modify ingredient with negative value
	Done		2.16) Modify missing ingredient
	Done		2.17) Get ingredient value by empty name
	Done		2.18) Get ingredient value by missing name
	Done	3) Ingredients iterators
	Done		3.1) Recipe with empty ingredients
	Done		3.2) Recipe with several ingredients
	Done	4) Cook step
	Done		4.1) Add cook step
	Done		4.2) Add several cook steps
	Done		4.3) Add several cook steps with the same name
	Done		4.4) Add cook step with empty name
	Done		4.5) Get cook step by negative index
	Done		4.6) Get cook step by out-of-range index
	Done	5) Compare recipes
	Done		5.1) Equal recipes without ingredients and cook steps
	Done		5.2) Different names
	Done		5.3) Different description
	Done		5.4) Different authors
	Done		5.5) Different ingredients count
	Done		5.6) Different ingredient names
	Done		5.7) The same ingredient but different value
	Done		5.8) Different cook steps count
	Done		5.9) Different cook steps
	Done		5.10) Different order of cook steps
	Done		5.11) Equal complex recipes
*/

/*****************************************************************************/


DECLARE_OOP_TEST( recipe_1_1_basic_simple_recipe )
{
	const std::string name = "salad";
	const std::string description = "magic salad";
	const std::string authorName = "kris";

	const Recipe r( name, description, authorName );

	assert( r.getName() == name );
	assert( r.getDescription() == description );
	assert( r.getAuthor() == authorName );
	assert( r.getIngredientsCount() == 0 );
	assert( r.getCookStepsCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_1_2_basic_several_recipes )
{
	const std::string name1 = "salad";
	const std::string description1 = "magic salad";
	const std::string authorName1 = "kris";

	const std::string name2 = "fish";
	const std::string description2 = "there is some description";
	const std::string authorName2 = "unknown";

	const Recipe r1( name1, description1, authorName1 );
	const Recipe r2( name2, description2, authorName2 );

	assert( r1.getName() == name1 );
	assert( r1.getDescription() == description1 );
	assert( r1.getAuthor() == authorName1 );
	assert( r1.getIngredientsCount() == 0 );
	assert( r1.getCookStepsCount() == 0 );

	assert( r2.getName() == name2 );
	assert( r2.getDescription() == description2 );
	assert( r2.getAuthor() == authorName2 );
	assert( r2.getIngredientsCount() == 0 );
	assert( r2.getCookStepsCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_1_3_basic_several_recipes_with_the_same_name )
{
	const std::string name1 = "recipe";
	const std::string description1 = "hello, mr. anderson";
	const std::string authorName1 = "smith";

	const std::string name2 = "recipe";
	const std::string description2 = "new recipe 1";
	const std::string authorName2 = "batman";

	const Recipe r1( name1, description1, authorName1 );
	const Recipe r2( name2, description2, authorName2 );

	assert( r1.getName() == name1 );
	assert( r1.getDescription() == description1 );
	assert( r1.getAuthor() == authorName1 );
	assert( r1.getIngredientsCount() == 0 );
	assert( r1.getCookStepsCount() == 0 );

	assert( r2.getName() == name2 );
	assert( r2.getDescription() == description2 );
	assert( r2.getAuthor() == authorName2 );
	assert( r2.getIngredientsCount() == 0 );
	assert( r2.getCookStepsCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_1_4_basic_create_recipe_with_empty_name )
{
	const std::string name = "";
	const std::string description = "description";
	const std::string authorName = "author";

	ASSERT_THROWS(
			Recipe r( name, description, authorName )
		,	Messages::EmptyRecipeName
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_1_5_basic_create_recipe_with_empty_description )
{
	const std::string name = "name";
	const std::string description = "";
	const std::string authorName = "author";

	ASSERT_THROWS(
			Recipe r( name, description, authorName )
		,	Messages::EmptyRecipeDescription
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_1_6_basic_create_recipe_with_empty_author_name )
{
	const std::string name = "name";
	const std::string description = "description";
	const std::string authorName = "";

	ASSERT_THROWS(
			Recipe r( name, description, authorName )
		,	Messages::EmptyRecipeAuthor
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_1_ingredient_add_ingredient )
{
	const std::string ingredientName = "ingredient";
	const int ingredientValue = 97;

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName, ingredientValue );
	
	assert( r.getIngredientsCount() == 1 );
	assert( r.getIngredientValue( ingredientName ) == ingredientValue );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_2_ingredient_add_several_ingredients )
{
	const std::string ingredientName1 = "ingredient_1";
	const int ingredientValue1 = 97;
	const std::string ingredientName2 = "ingredient_2";
	const int ingredientValue2 = 53;

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName1, ingredientValue1 );
	r.addIngredient( ingredientName2, ingredientValue2 );
	
	assert( r.getIngredientsCount() == 2 );
	assert( r.getIngredientValue( ingredientName1 ) == ingredientValue1 );
	assert( r.getIngredientValue( ingredientName2 ) == ingredientValue2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_3_ingredient_add_several_ingredients_with_the_same_name )
{
	const std::string ingredientName1 = "ingredient_1";
	const int ingredientValue1 = 97;
	const std::string ingredientName2 = "ingredient_1";
	const int ingredientValue2 = 53;

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName1, ingredientValue1 );

	ASSERT_THROWS(
			r.addIngredient( ingredientName2, ingredientValue2 );
		,	Messages::DuplicateIngredient
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_4_ingredient_add_ingredient_with_empty_name )
{
	const std::string ingredientName = "";
	const int ingredientValue = 97;

	Recipe r( "name", "description", "author" );

	ASSERT_THROWS(
			r.addIngredient( ingredientName, ingredientValue );
		,	Messages::EmptyIngredientName
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_5_ingredient_add_ingredient_with_zero_value )
{
	const std::string ingredientName = "ingredient_1";
	const int ingredientValue = 0;

	Recipe r( "name", "description", "author" );

	ASSERT_THROWS(
			r.addIngredient( ingredientName, ingredientValue );
		,	Messages::IngredientValueMustBePositive
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_6_ingredient_add_ingredient_with_negative_value )
{
	const std::string ingredientName = "ingredient_1";
	const int ingredientValue = -10;

	Recipe r( "name", "description", "author" );

	ASSERT_THROWS(
			r.addIngredient( ingredientName, ingredientValue );
		,	Messages::IngredientValueMustBePositive
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_7_ingredient_has_ingredient )
{
	const std::string ingredientName = "ingredient_55";
	const int ingredientValue = 34;

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName, ingredientValue );

	assert( r.hasIngredient( ingredientName ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_8_ingredient_has_ingredient_by_empty_name )
{
	const std::string ingredientName = "ingredient_11";
	const int ingredientValue = 34;

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName, ingredientValue );

	assert( !r.hasIngredient( "" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_9_ingredient_has_ingredient_byr_missing_name )
{
	const std::string ingredientName = "ingredient_78";
	const int ingredientValue = 34;

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName, ingredientValue );

	assert( !r.hasIngredient( "missing" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_10_ingredient_modify_ingredient )
{
	const std::string ingredientName = "ingredient_660";

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName, 3 );
	r.modifyIngredient( ingredientName, 57 );
	
	assert( r.getIngredientsCount() == 1 );
	assert( r.getIngredientValue( ingredientName ) == 57 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_11_ingredient_modify_several_ingredients )
{
	const std::string ingredientName1 = "ingredient_1";
	const std::string ingredientName2 = "ingredient_2";

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName1, 43634 );
	r.addIngredient( ingredientName2, 36456 );
	r.modifyIngredient( ingredientName1, 5566 );
	r.modifyIngredient( ingredientName2, 7788 );
	
	assert( r.getIngredientsCount() == 2 );
	assert( r.getIngredientValue( ingredientName1 ) == 5566 );
	assert( r.getIngredientValue( ingredientName2 ) == 7788 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_12_ingredient_modify_ingredient_several_times )
{
	const std::string ingredientName = "ingredient";

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName, 111 );
	r.modifyIngredient( ingredientName, 222 );
	r.modifyIngredient( ingredientName, 333 );
	
	assert( r.getIngredientsCount() == 1 );
	assert( r.getIngredientValue( ingredientName ) == 333 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_13_ingredient_modify_ingredient_with_empty_name )
{
	const std::string ingredientName = "ingredient";

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName, 111 );

	ASSERT_THROWS(
			r.modifyIngredient( "", 222 );
		,	Messages::EmptyIngredientName
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_14_ingredient_modify_ingredient_with_zero_value )
{
	const std::string ingredientName = "ingredient";

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName, 111 );

	ASSERT_THROWS(
			r.modifyIngredient( ingredientName, 0 );
		,	Messages::IngredientValueMustBePositive
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_15_ingredient_modify_ingredient_with_negative_value )
{
	const std::string ingredientName = "ingredient";

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName, 111 );

	ASSERT_THROWS(
			r.modifyIngredient( ingredientName, -22 );
		,	Messages::IngredientValueMustBePositive
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_16_ingredient_modify_missing_ingredient )
{
	const std::string ingredientName = "ingredient";

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName, 111 );

	ASSERT_THROWS(
			r.modifyIngredient( "missing", 222 );
		,	Messages::IngredientCannotBeFound
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_17_ingredient_get_ingredient_value_by_empty_name )
{
	const std::string ingredientName = "ingredient";
	const int ingredientValue = 111;

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName, ingredientValue );

	ASSERT_THROWS(
			r.getIngredientValue( "" );
		,	Messages::EmptyIngredientName
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_2_18_ingredient_get_ingredient_value_by_missing_name )
{
	const std::string ingredientName = "ingredient";
	const int ingredientValue = 111;

	Recipe r( "name", "description", "author" );
	r.addIngredient( ingredientName, ingredientValue );

	ASSERT_THROWS(
			r.getIngredientValue( "missing" );
		,	Messages::IngredientCannotBeFound
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_3_1_ingredient_iterators_recipe_with_empty_ingredients )
{
	const Recipe r( "name", "description", "author" );

	auto it = r.beginIngredients();
	auto itEnd = r.endIngredients();
	for ( ; it != itEnd; ++it )
		assert( !"This code should not be executed" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_3_2_ingredient_iterators_recipe_with_several_ingredients )
{
	Recipe r( "name", "description", "author" );
	r.addIngredient( "ingredient1", 1 );
	r.addIngredient( "ingredient2", 2 );
	r.addIngredient( "ingredient3", 3 );

	std::unordered_set< std::string > expectation;
	expectation.insert( "ingredient1" );
	expectation.insert( "ingredient2" );
	expectation.insert( "ingredient3" );

	auto it = r.beginIngredients();
	auto itEnd = r.endIngredients();
	for ( ; it != itEnd; ++it )
		assert( expectation.count( it->first ) == 1 );

	assert( r.getIngredientsCount() == 3 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_4_1_cook_step_add_cook_step )
{
	Recipe r( "name", "description", "author" );
	r.addIngredient( "ingredient1", 1 );
	r.addIngredient( "ingredient2", 2 );
	r.addIngredient( "ingredient3", 3 );

	r.addCookStep( "step_1" );
	assert( r.getCookStepsCount() == 1 );
	assert( r.getCookStep( 0 ) == "step_1" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_4_2_cook_step_add_several_cook_steps )
{
	Recipe r( "name", "description", "author" );
	r.addIngredient( "ingredient1", 1 );
	r.addIngredient( "ingredient2", 2 );
	r.addIngredient( "ingredient3", 3 );

	r.addCookStep( "step_1" );
	r.addCookStep( "step_2" );
	r.addCookStep( "step_3" );
	assert( r.getCookStepsCount() == 3 );
	assert( r.getCookStep( 0 ) == "step_1" );
	assert( r.getCookStep( 1 ) == "step_2" );
	assert( r.getCookStep( 2 ) == "step_3" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_4_3_cook_step_add_several_cook_steps_with_the_same_name )
{
	Recipe r( "name", "description", "author" );
	r.addIngredient( "ingredient1", 1 );
	r.addIngredient( "ingredient2", 2 );
	r.addIngredient( "ingredient3", 3 );

	r.addCookStep( "step" );
	r.addCookStep( "step" );
	r.addCookStep( "step" );
	assert( r.getCookStepsCount() == 3 );
	assert( r.getCookStep( 0 ) == "step" );
	assert( r.getCookStep( 1 ) == "step" );
	assert( r.getCookStep( 2 ) == "step" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_4_4_cook_step_add_cook_step_with_empty_name )
{
	Recipe r( "name", "description", "author" );
	r.addIngredient( "ingredient1", 1 );
	r.addIngredient( "ingredient2", 2 );
	r.addIngredient( "ingredient3", 3 );

	ASSERT_THROWS(
			r.addCookStep( "" );
		,	Messages::EmptyCookStep
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_4_5_cook_step_get_cook_step_by_negative_index )
{
	Recipe r( "name", "description", "author" );
	r.addIngredient( "ingredient1", 1 );
	r.addIngredient( "ingredient2", 2 );
	r.addIngredient( "ingredient3", 3 );

	r.addCookStep( "step" );
	r.addCookStep( "step" );
	r.addCookStep( "step" );
	assert( r.getCookStepsCount() == 3 );

	ASSERT_THROWS(
			r.getCookStep( -1 )
		,	Messages::IndexOutOfRange
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_4_6_cook_step_get_cook_step_by_outofrange_index )
{
	Recipe r( "name", "description", "author" );
	r.addIngredient( "ingredient1", 1 );
	r.addIngredient( "ingredient2", 2 );
	r.addIngredient( "ingredient3", 3 );

	r.addCookStep( "step" );
	r.addCookStep( "step" );
	r.addCookStep( "step" );
	assert( r.getCookStepsCount() == 3 );

	ASSERT_THROWS(
			r.getCookStep( 3 )
		,	Messages::IndexOutOfRange
	);
	ASSERT_THROWS(
			r.getCookStep( 4 )
		,	Messages::IndexOutOfRange
	);
	ASSERT_THROWS(
			r.getCookStep( 5 )
		,	Messages::IndexOutOfRange
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_5_1_compare_recipes_equal_recipes_without_ingredients_and_cook_steps )
{
	const Recipe r1( "salad", "magic salad", "kris" );
	const Recipe r2( "salad", "magic salad", "kris" );

	assert( r1 == r2 );
	assert( !( r1 != r2 ) );

	assert( r1 == r1 );
	assert( !( r1 != r1 ) );

	assert( r2 == r2 );
	assert( !( r2 != r2 ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_5_2_compare_recipes_different_names )
{
	const Recipe r1( "salad 1", "magic salad", "kris" );
	const Recipe r2( "salad 2", "magic salad", "kris" );

	assert( r1 != r2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_5_3_compare_recipes_different_description )
{
	const Recipe r1( "salad", "magic salad 1", "kris" );
	const Recipe r2( "salad", "magic salad 2", "kris" );

	assert( r1 != r2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_5_4_compare_recipes_different_authors )
{
	const Recipe r1( "salad", "magic salad", "you" );
	const Recipe r2( "salad", "magic salad", "me" );

	assert( r1 != r2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_5_5_compare_recipes_different_ingredients_count )
{
	Recipe r1( "salad", "magic salad", "kris" );
	Recipe r2( "salad", "magic salad", "kris" );

	r1.addIngredient( "i1", 100 );
	r1.addIngredient( "i2", 100 );

	r2.addIngredient( "i1", 100 );

	assert( r1 != r2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_5_6_compare_recipes_different_ingredients )
{
	Recipe r1( "salad", "magic salad", "kris" );
	Recipe r2( "salad", "magic salad", "kris" );

	r1.addIngredient( "i10", 100 );
	r2.addIngredient( "i20", 100 );

	assert( r1 != r2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_5_7_compare_recipes_the_same_ingredient_but_different_value )
{
	Recipe r1( "salad", "magic salad", "kris" );
	Recipe r2( "salad", "magic salad", "kris" );

	r1.addIngredient( "i", 100 );
	r2.addIngredient( "i", 200 );

	assert( r1 != r2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_5_8_compare_recipes_different_cook_steps_count )
{
	Recipe r1( "salad", "magic salad", "kris" );
	Recipe r2( "salad", "magic salad", "kris" );

	r1.addCookStep( "step1" );
	r1.addCookStep( "step2" );

	r2.addCookStep( "step1" );

	assert( r1 != r2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_5_9_compare_recipes_different_cook_steps )
{
	Recipe r1( "salad", "magic salad", "kris" );
	Recipe r2( "salad", "magic salad", "kris" );

	r1.addCookStep( "step1" );
	r1.addCookStep( "step2" );

	r2.addCookStep( "step3" );
	r2.addCookStep( "step4" );

	assert( r1 != r2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_5_10_compare_recipes_different_order_of_cook_steps )
{
	Recipe r1( "salad", "magic salad", "kris" );
	Recipe r2( "salad", "magic salad", "kris" );

	r1.addCookStep( "step1" );
	r1.addCookStep( "step2" );

	r2.addCookStep( "step2" );
	r2.addCookStep( "step1" );

	assert( r1 != r2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( recipe_5_11_compare_recipes_equal_complex_recipes )
{
	Recipe r1( "salad", "magic salad", "kris" );
	Recipe r2( "salad", "magic salad", "kris" );

	r1.addIngredient( "ingredient1", 5 );
	r1.addIngredient( "ingredient2", 10 );
	r1.addCookStep( "step1" );
	r1.addCookStep( "step2" );

	r2.addIngredient( "ingredient1", 5 );
	r2.addIngredient( "ingredient2", 10 );
	r2.addCookStep( "step1" );
	r2.addCookStep( "step2" );

	assert( r1 == r2 );
	assert( !( r1 != r2 ) );
}


/*****************************************************************************/
