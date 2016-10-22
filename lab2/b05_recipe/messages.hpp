// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _MESSAGES_HPP_
#define _MESSAGES_HPP_

/*****************************************************************************/

namespace Messages
{
	const char * const EmptyRecipeName = "Recipe name cannot be empty";
	const char * const EmptyRecipeDescription = "Recipe description cannot be empty";
	const char * const EmptyRecipeAuthor = "Recipe author cannot be empty";
	const char * const EmptyIngredientName = "Ingredient name cannot be empty";
	const char * const EmptyCookStep = "Cook step cannot be empty";
	const char * const IngredientValueMustBePositive = "Ingredient value must be positive";
	const char * const DuplicateIngredient = "Ingredient must be unique";
	const char * const IngredientCannotBeFound = "Ingredient cannot be found";
	const char * const IndexOutOfRange = "Index out of range";
}

/*****************************************************************************/

#endif // _MESSAGES_HPP_
