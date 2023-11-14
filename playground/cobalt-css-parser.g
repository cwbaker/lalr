//From: https://github.com/youtube/cobalt/blob/main/cobalt/css_parser/grammar.y

cobalt_css {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n\r]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

/*Tokens*/
//%token kEndOfFileToken
//%token YYerror
//%token YYUNDEF
//%token kMediaListEntryPointToken
//%token kMediaQueryEntryPointToken
//%token kStyleSheetEntryPointToken
//%token kRuleEntryPointToken
//%token kStyleDeclarationListEntryPointToken
//%token kFontFaceDeclarationListEntryPointToken
//%token kPropertyValueEntryPointToken
//%token kPropertyIntoDeclarationDataEntryPointToken
//%token kWhitespaceToken
//%token kSgmlCommentDelimiterToken
//%token kCommentToken
//%token kImportantToken
//%token kAllToken
//%token kAlignContentToken
//%token kAlignItemsToken
//%token kAlignSelfToken
//%token kAnimationDelayToken
//%token kAnimationDirectionToken
//%token kAnimationDurationToken
//%token kAnimationFillModeToken
//%token kAnimationIterationCountToken
//%token kAnimationNameToken
//%token kAnimationTimingFunctionToken
//%token kAnimationToken
//%token kBackgroundColorToken
//%token kBackgroundImageToken
//%token kBackgroundPositionToken
//%token kBackgroundRepeatToken
//%token kBackgroundSizeToken
//%token kBackgroundToken
//%token kBorderToken
//%token kBorderBottomLeftRadiusToken
//%token kBorderBottomRightRadiusToken
//%token kBorderBottomToken
//%token kBorderBottomColorToken
//%token kBorderBottomStyleToken
//%token kBorderBottomWidthToken
//%token kBorderColorToken
//%token kBorderLeftToken
//%token kBorderLeftColorToken
//%token kBorderLeftStyleToken
//%token kBorderLeftWidthToken
//%token kBorderRadiusToken
//%token kBorderRightToken
//%token kBorderRightColorToken
//%token kBorderRightStyleToken
//%token kBorderRightWidthToken
//%token kBorderStyleToken
//%token kBorderTopToken
//%token kBorderTopColorToken
//%token kBorderTopLeftRadiusToken
//%token kBorderTopRightRadiusToken
//%token kBorderTopStyleToken
//%token kBorderTopWidthToken
//%token kBorderWidthToken
//%token kBottomToken
//%token kBoxShadowToken
//%token kColorToken
//%token kContentToken
//%token kDisplayToken
//%token kFilterToken
//%token kFlexToken
//%token kFlexBasisToken
//%token kFlexDirectionToken
//%token kFlexFlowToken
//%token kFlexGrowToken
//%token kFlexShrinkToken
//%token kFlexWrapToken
//%token kFontToken
//%token kFontFamilyToken
//%token kFontSizeToken
//%token kFontStyleToken
//%token kFontWeightToken
//%token kHeightToken
//%token kIntersectionObserverRootMarginToken
//%token kJustifyContentToken
//%token kLeftToken
//%token kLineHeightToken
//%token kMarginBottomToken
//%token kMarginLeftToken
//%token kMarginRightToken
//%token kMarginToken
//%token kMarginTopToken
//%token kMaxHeightToken
//%token kMaxWidthToken
//%token kMinHeightToken
//%token kMinWidthToken
//%token kOpacityToken
//%token kOrderToken
//%token kOutlineToken
//%token kOutlineColorToken
//%token kOutlineStyleToken
//%token kOutlineWidthToken
//%token kOverflowToken
//%token kOverflowWrapToken
//%token kPaddingBottomToken
//%token kPaddingLeftToken
//%token kPaddingRightToken
//%token kPaddingToken
//%token kPaddingTopToken
//%token kPointerEventsToken
//%token kPositionToken
//%token kRightToken
//%token kSrcToken
//%token kTextAlignToken
//%token kTextDecorationToken
//%token kTextDecorationColorToken
//%token kTextDecorationLineToken
//%token kTextIndentToken
//%token kTextOverflowToken
//%token kTextShadowToken
//%token kTextTransformToken
//%token kTopToken
//%token kTransformToken
//%token kTransformOriginToken
//%token kTransitionDelayToken
//%token kTransitionDurationToken
//%token kTransitionPropertyToken
//%token kTransitionTimingFunctionToken
//%token kTransitionToken
//%token kUnicodeRangePropertyToken
//%token kVerticalAlignToken
//%token kVisibilityToken
//%token kWhiteSpacePropertyToken
//%token kWidthToken
//%token kWordWrapToken
//%token kZIndexToken
//%token kAbsoluteToken
//%token kAlternateToken
//%token kAlternateReverseToken
//%token kAquaToken
//%token kAtToken
//%token kAutoToken
//%token kBackwardsToken
//%token kBaselineToken
//%token kBlackToken
//%token kBlockToken
//%token kBlueToken
//%token kBoldToken
//%token kBothToken
//%token kBreakWordToken
//%token kCenterToken
//%token kCircleToken
//%token kClipToken
//%token kClosestCornerToken
//%token kClosestSideToken
//%token kCollapseToken
//%token kColumnToken
//%token kColumnReverseToken
//%token kContainToken
//%token kCoverToken
//%token kCursiveToken
//%token kEaseInOutToken
//%token kEaseInToken
//%token kEaseOutToken
//%token kEaseToken
//%token kEllipseToken
//%token kEllipsisToken
//%token kEndToken
//%token kEquirectangularToken
//%token kFantasyToken
//%token kFarthestCornerToken
//%token kFarthestSideToken
//%token kFixedToken
//%token kFlexEndToken
//%token kFlexStartToken
//%token kForwardsToken
//%token kFromToken
//%token kFuchsiaToken
//%token kGrayToken
//%token kGreenToken
//%token kHiddenToken
//%token kInfiniteToken
//%token kInheritToken
//%token kInitialToken
//%token kInlineBlockToken
//%token kInlineToken
//%token kInlineFlexToken
//%token kInsetToken
//%token kItalicToken
//%token kLimeToken
//%token kLinearToken
//%token kLineThroughToken
//%token kMaroonToken
//%token kMiddleToken
//%token kMonoscopicToken
//%token kMonospaceToken
//%token kNavyToken
//%token kNoneToken
//%token kNoRepeatToken
//%token kNormalToken
//%token kNowrapToken
//%token kObliqueToken
//%token kOliveToken
//%token kPreToken
//%token kPreLineToken
//%token kPreWrapToken
//%token kPurpleToken
//%token kRectangularToken
//%token kRedToken
//%token kRepeatToken
//%token kRepeatXToken
//%token kRepeatYToken
//%token kRelativeToken
//%token kReverseToken
//%token kRowToken
//%token kRowReverseToken
//%token kSansSerifToken
//%token kScrollToken
//%token kSerifToken
//%token kSilverToken
//%token kSolidToken
//%token kSpaceAroundToken
//%token kSpaceBetweenToken
//%token kStartToken
//%token kStaticToken
//%token kStepEndToken
//%token kStepStartToken
//%token kStereoscopicLeftRightToken
//%token kStereoscopicTopBottomToken
//%token kStretchToken
//%token kTealToken
//%token kToToken
//%token kTransparentToken
//%token kUppercaseToken
//%token kVisibleToken
//%token kWhiteToken
//%token kWrapToken
//%token kWrapReverseToken
//%token kYellowToken
//%token kActiveToken
//%token kEmptyToken
//%token kFocusToken
//%token kHoverToken
//%token kAfterToken
//%token kBeforeToken
//%token kIncludesToken
//%token kDashMatchToken
//%token kBeginsWithToken
//%token kEndsWithToken
//%token kContainsToken
//%token kMediaAndToken
//%token kMediaNotToken
//%token kMediaOnlyToken
//%token kMediaMinimumToken
//%token kMediaMaximumToken
//%token kAllMediaTypeToken
//%token kTVMediaTypeToken
//%token kScreenMediaTypeToken
//%token kLengthMediaFeatureTypeToken
//%token kOrientationMediaFeatureTypeToken
//%token kRatioMediaFeatureTypeToken
//%token kNonNegativeIntegerMediaFeatureTypeToken
//%token kResolutionMediaFeatureTypeToken
//%token kScanMediaFeatureTypeToken
//%token kZeroOrOneMediaFeatureTypeToken
//%token kInterlaceMediaFeatureKeywordValueToken
//%token kLandscapeMediaFeatureKeywordValueToken
//%token kPortraitMediaFeatureKeywordValueToken
//%token kProgressiveMediaFeatureKeywordValueToken
//%token kSupportsAndToken
//%token kSupportsNotToken
//%token kSupportsOrToken
//%token kImportToken
//%token kKeyframesToken
//%token kPageToken
//%token kMediaToken
//%token kFontFaceToken
//%token kCharsetToken
//%token kNamespaceToken
//%token kSupportsToken
//%token kTopLeftCornerToken
//%token kTopLeftToken
//%token kTopCenterToken
//%token kTopRightToken
//%token kTopRightCornerToken
//%token kBottomLeftCornerToken
//%token kBottomLeftToken
//%token kBottomCenterToken
//%token kBottomRightToken
//%token kBottomRightCornerToken
//%token kLeftTopToken
//%token kLeftMiddleToken
//%token kLeftBottomToken
//%token kRightTopToken
//%token kRightMiddleToken
//%token kRightBottomToken
//%token kCalcFunctionToken
//%token kCubicBezierFunctionToken
//%token kCueFunctionToken
//%token kFormatFunctionToken
//%token kHslFunctionToken
//%token kHslaFunctionToken
//%token kLinearGradientFunctionToken
//%token kLocalFunctionToken
//%token kMapToMeshFunctionToken
//%token kMatrixFunctionToken
//%token kMatrix3dFunctionToken
//%token kNotFunctionToken
//%token kNthChildFunctionToken
//%token kNthLastChildFunctionToken
//%token kNthLastOfTypeFunctionToken
//%token kNthOfTypeFunctionToken
//%token kRotateFunctionToken
//%token kScaleFunctionToken
//%token kScaleXFunctionToken
//%token kScaleYFunctionToken
//%token kStepsFunctionToken
//%token kTranslateFunctionToken
//%token kTranslateXFunctionToken
//%token kTranslateYFunctionToken
//%token kTranslateZFunctionToken
//%token kRadialGradientFunctionToken
//%token kRGBFunctionToken
//%token kRGBAFunctionToken
//%token kCobaltMtmFunctionToken
//%token kCobaltUiNavFocusTransformFunctionToken
//%token kCobaltUiNavSpotlightTransformFunctionToken
//%token kStringToken
//%token kIdentifierToken
//%token kNthToken
//%token kHexToken
//%token kIdSelectorToken
//%token kUriToken
//%token kInvalidFunctionToken
//%token kInvalidNumberToken
//%token kInvalidDimensionToken
//%token kInvalidAtBlockToken
//%token kOtherBrowserAtBlockToken
//%token kIntegerToken
//%token kRealToken
//%token kPercentageToken
//%token kRootElementFontSizesAkaRemToken
//%token kZeroGlyphWidthsAkaChToken
//%token kFontSizesAkaEmToken
//%token kXHeightsAkaExToken
//%token kPixelsToken
//%token kCentimetersToken
//%token kMillimetersToken
//%token kInchesToken
//%token kPointsToken
//%token kPicasToken
//%token kDegreesToken
//%token kRadiansToken
//%token kGradiansToken
//%token kTurnsToken
//%token kMillisecondsToken
//%token kSecondsToken
//%token kHertzToken
//%token kKilohertzToken
//%token kViewportWidthPercentsAkaVwToken
//%token kViewportHeightPercentsAkaVhToken
//%token kViewportSmallerSizePercentsAkaVminToken
//%token kViewportLargerSizePercentsAkaVmaxToken
//%token kDotsPerPixelToken
//%token kDotsPerInchToken
//%token kDotsPerCentimeterToken
//%token kFractionsToken
//%token kUnicodeRangeToken
//%token '{'
//%token '}'
//%token '('
//%token ')'
//%token ','
//%token '*'
//%token '='
//%token '['
//%token ']'
//%token '.'
//%token ':'
//%token '>'
//%token '+'
//%token '~'
//%token '-'
//%token '/'
//%token ';'


//%start entry_point

//%%

entry_point :
	kEndOfFileToken
	;

maybe_whitespace :
	/*empty*/
	| maybe_whitespace kWhitespaceToken
	;

errors :
	error
	| errors error
	;

at_font_face_rule :
	kFontFaceToken maybe_whitespace '{' maybe_whitespace font_face_declaration_list '}' maybe_whitespace
	;

at_media_rule :
	kMediaToken maybe_whitespace media_list rule_list_block
	;

orientation_media_feature_keyword_value :
	kLandscapeMediaFeatureKeywordValueToken maybe_whitespace
	| kPortraitMediaFeatureKeywordValueToken maybe_whitespace
	;

scan_media_feature_keyword_value :
	kInterlaceMediaFeatureKeywordValueToken maybe_whitespace
	| kProgressiveMediaFeatureKeywordValueToken maybe_whitespace
	;

media_feature_operator :
	kMediaMinimumToken
	| kMediaMaximumToken
	;

media_feature_without_value :
	kLengthMediaFeatureTypeToken maybe_whitespace
	| kOrientationMediaFeatureTypeToken maybe_whitespace
	| kRatioMediaFeatureTypeToken maybe_whitespace
	| kNonNegativeIntegerMediaFeatureTypeToken maybe_whitespace
	| kResolutionMediaFeatureTypeToken maybe_whitespace
	| kScanMediaFeatureTypeToken maybe_whitespace
	| kZeroOrOneMediaFeatureTypeToken maybe_whitespace
	;

media_feature_with_value :
	kOrientationMediaFeatureTypeToken maybe_whitespace colon orientation_media_feature_keyword_value
	| kScanMediaFeatureTypeToken maybe_whitespace colon scan_media_feature_keyword_value
	| kZeroOrOneMediaFeatureTypeToken maybe_whitespace colon zero_or_one
	;

media_feature_allowing_operator_with_value :
	kLengthMediaFeatureTypeToken maybe_whitespace colon length
	| kNonNegativeIntegerMediaFeatureTypeToken maybe_whitespace colon non_negative_integer
	| kRatioMediaFeatureTypeToken maybe_whitespace colon ratio
	| kResolutionMediaFeatureTypeToken maybe_whitespace colon resolution
	;

media_feature :
	media_feature_without_value
	| media_feature_with_value
	| media_feature_allowing_operator_with_value
	| media_feature_operator media_feature_allowing_operator_with_value
	;

media_feature_block :
	'(' maybe_whitespace media_feature ')' maybe_whitespace
	;

media_feature_list :
	media_feature_block
	| media_feature_list kMediaAndToken maybe_whitespace media_feature_block
	;

media_type_unknown :
	kIdentifierToken
	| kMediaAndToken
	| kMediaMinimumToken
	| kMediaMaximumToken
	| kLengthMediaFeatureTypeToken
	| kOrientationMediaFeatureTypeToken
	| kRatioMediaFeatureTypeToken
	| kNonNegativeIntegerMediaFeatureTypeToken
	| kResolutionMediaFeatureTypeToken
	| kScanMediaFeatureTypeToken
	| kZeroOrOneMediaFeatureTypeToken
	;

media_type_known :
	kAllMediaTypeToken
	| kTVMediaTypeToken
	| kScreenMediaTypeToken
	;

media_type_specified :
	media_type_unknown
	| media_type_known
	;

evaluated_media_type :
	media_type_specified
	| kMediaNotToken kWhitespaceToken media_type_specified
	| kMediaOnlyToken kWhitespaceToken media_type_specified
	| kMediaOnlyToken kWhitespaceToken kMediaNotToken
	| kMediaNotToken kWhitespaceToken kMediaOnlyToken
	;

media_query :
	/*empty*/
	| media_feature_list
	| evaluated_media_type maybe_whitespace
	| evaluated_media_type maybe_whitespace kMediaAndToken maybe_whitespace media_feature_list
	| errors
	;

media_list :
	media_query
	| media_list comma media_query
	;

at_keyframes_rule :
	kKeyframesToken maybe_whitespace kIdentifierToken maybe_whitespace '{' maybe_whitespace keyframe_rule_list '}' maybe_whitespace
	;

keyframe_rule_list :
	keyframe_rule
	| error
	| keyframe_rule_list keyframe_rule
	;

keyframe_rule :
	keyframe_selector style_declaration_block
	;

keyframe_selector :
	keyframe_offset
	| keyframe_selector ',' maybe_whitespace keyframe_offset
	;

keyframe_offset :
	kFromToken maybe_whitespace
	| kToToken maybe_whitespace
	| kPercentageToken maybe_whitespace
	;

identifier_token :
	kIdentifierToken
	| kAllToken
	| kAlignContentToken
	| kAlignItemsToken
	| kAlignSelfToken
	| kAnimationDelayToken
	| kAnimationDirectionToken
	| kAnimationDurationToken
	| kAnimationFillModeToken
	| kAnimationIterationCountToken
	| kAnimationNameToken
	| kAnimationTimingFunctionToken
	| kAnimationToken
	| kBackgroundColorToken
	| kBackgroundImageToken
	| kBackgroundPositionToken
	| kBackgroundRepeatToken
	| kBackgroundSizeToken
	| kBackgroundToken
	| kBorderToken
	| kBorderBottomLeftRadiusToken
	| kBorderBottomRightRadiusToken
	| kBorderBottomToken
	| kBorderBottomColorToken
	| kBorderBottomStyleToken
	| kBorderBottomWidthToken
	| kBorderLeftToken
	| kBorderColorToken
	| kBorderLeftColorToken
	| kBorderLeftStyleToken
	| kBorderLeftWidthToken
	| kBorderRadiusToken
	| kBorderRightToken
	| kBorderRightColorToken
	| kBorderRightStyleToken
	| kBorderRightWidthToken
	| kBorderStyleToken
	| kBorderTopToken
	| kBorderTopColorToken
	| kBorderTopLeftRadiusToken
	| kBorderTopRightRadiusToken
	| kBorderTopStyleToken
	| kBorderTopWidthToken
	| kBorderWidthToken
	| kBottomToken
	| kBoxShadowToken
	| kColorToken
	| kContentToken
	| kDisplayToken
	| kFilterToken
	| kFlexToken
	| kFlexBasisToken
	| kFlexDirectionToken
	| kFlexFlowToken
	| kFlexGrowToken
	| kFlexShrinkToken
	| kFlexWrapToken
	| kFontToken
	| kFontFamilyToken
	| kFontSizeToken
	| kFontStyleToken
	| kFontWeightToken
	| kHeightToken
	| kIntersectionObserverRootMarginToken
	| kJustifyContentToken
	| kLeftToken
	| kLineHeightToken
	| kMarginBottomToken
	| kMarginLeftToken
	| kMarginRightToken
	| kMarginToken
	| kMarginTopToken
	| kMaxHeightToken
	| kMaxWidthToken
	| kMinHeightToken
	| kMinWidthToken
	| kOpacityToken
	| kOrderToken
	| kOutlineToken
	| kOutlineColorToken
	| kOutlineStyleToken
	| kOutlineWidthToken
	| kOverflowToken
	| kOverflowWrapToken
	| kPaddingBottomToken
	| kPaddingLeftToken
	| kPaddingRightToken
	| kPaddingToken
	| kPaddingTopToken
	| kPointerEventsToken
	| kPositionToken
	| kRightToken
	| kSrcToken
	| kTextAlignToken
	| kTextDecorationToken
	| kTextDecorationColorToken
	| kTextDecorationLineToken
	| kTextIndentToken
	| kTextOverflowToken
	| kTextShadowToken
	| kTextTransformToken
	| kTopToken
	| kTransformToken
	| kTransformOriginToken
	| kTransitionDelayToken
	| kTransitionDurationToken
	| kTransitionPropertyToken
	| kTransitionTimingFunctionToken
	| kTransitionToken
	| kUnicodeRangePropertyToken
	| kVerticalAlignToken
	| kVisibilityToken
	| kWhiteSpacePropertyToken
	| kWidthToken
	| kWordWrapToken
	| kZIndexToken
	| kAbsoluteToken
	| kAlternateToken
	| kAlternateReverseToken
	| kAquaToken
	| kAtToken
	| kAutoToken
	| kBackwardsToken
	| kBaselineToken
	| kBlackToken
	| kBlockToken
	| kBlueToken
	| kBoldToken
	| kBothToken
	| kBreakWordToken
	| kCenterToken
	| kCircleToken
	| kClipToken
	| kClosestCornerToken
	| kClosestSideToken
	| kCollapseToken
	| kColumnToken
	| kColumnReverseToken
	| kContainToken
	| kCoverToken
	| kCursiveToken
	| kEaseInOutToken
	| kEaseInToken
	| kEaseOutToken
	| kEaseToken
	| kEllipseToken
	| kEllipsisToken
	| kEndToken
	| kEquirectangularToken
	| kFantasyToken
	| kFarthestCornerToken
	| kFarthestSideToken
	| kFixedToken
	| kFlexEndToken
	| kFlexStartToken
	| kForwardsToken
	| kFromToken
	| kFuchsiaToken
	| kGrayToken
	| kGreenToken
	| kHiddenToken
	| kInfiniteToken
	| kInheritToken
	| kInitialToken
	| kInlineBlockToken
	| kInlineToken
	| kInlineFlexToken
	| kInsetToken
	| kItalicToken
	| kLimeToken
	| kLinearToken
	| kLineThroughToken
	| kMaroonToken
	| kMiddleToken
	| kMonoscopicToken
	| kMonospaceToken
	| kNavyToken
	| kNoneToken
	| kNoRepeatToken
	| kNormalToken
	| kNowrapToken
	| kObliqueToken
	| kOliveToken
	| kPreToken
	| kPreLineToken
	| kPreWrapToken
	| kPurpleToken
	| kRedToken
	| kRepeatToken
	| kRepeatXToken
	| kRepeatYToken
	| kRelativeToken
	| kReverseToken
	| kRowToken
	| kRowReverseToken
	| kSansSerifToken
	| kScrollToken
	| kSerifToken
	| kSilverToken
	| kSolidToken
	| kSpaceAroundToken
	| kSpaceBetweenToken
	| kStartToken
	| kStaticToken
	| kStepEndToken
	| kStepStartToken
	| kStereoscopicLeftRightToken
	| kStereoscopicTopBottomToken
	| kStretchToken
	| kTealToken
	| kToToken
	| kTransparentToken
	| kUppercaseToken
	| kVisibleToken
	| kWhiteToken
	| kWrapToken
	| kWrapReverseToken
	| kYellowToken
	| kActiveToken
	| kEmptyToken
	| kFocusToken
	| kHoverToken
	| kAfterToken
	| kBeforeToken
	;

universal_selector_token :
	'*'
	;

type_selector_token :
	identifier_token
	;

attribute_match :
	'=' maybe_whitespace
	| kIncludesToken maybe_whitespace
	| kDashMatchToken maybe_whitespace
	| kBeginsWithToken maybe_whitespace
	| kEndsWithToken maybe_whitespace
	| kContainsToken maybe_whitespace
	;

attribute_selector_token :
	'[' maybe_whitespace identifier_token maybe_whitespace ']'
	| '[' maybe_whitespace identifier_token maybe_whitespace attribute_match kStringToken maybe_whitespace ']'
	| '[' maybe_whitespace identifier_token maybe_whitespace attribute_match identifier_token maybe_whitespace ']'
	;

class_selector_token :
	'.' identifier_token
	;

id_selector_token :
	kIdSelectorToken
	| kHexToken
	;

pseudo_class_token :
	':' kActiveToken
	| ':' kEmptyToken
	| ':' kFocusToken
	| ':' kHoverToken
	| ':' kNotFunctionToken compound_selector_token ')'
	| ':' kNotFunctionToken errors ')'
	| ':' errors
	;

pseudo_element_token :
	':' kAfterToken
	| ':' kBeforeToken
	| ':' ':' kAfterToken
	| ':' ':' kBeforeToken
	| ':' ':' errors
	;

simple_selector_token :
	attribute_selector_token
	| class_selector_token
	| id_selector_token
	| pseudo_class_token
	| pseudo_element_token
	| type_selector_token
	| universal_selector_token
	;

compound_selector_token :
	simple_selector_token
	| compound_selector_token simple_selector_token
	;

combinator :
	kWhitespaceToken
	| '>' maybe_whitespace
	| '+' maybe_whitespace
	| '~' maybe_whitespace
	;

complex_selector :
	compound_selector_token
	| complex_selector combinator compound_selector_token
	| complex_selector kWhitespaceToken
	;

selector_list :
	complex_selector
	| selector_list comma complex_selector
	;

auto :
	kAutoToken maybe_whitespace
	;

maybe_sign_token :
	/*empty*/
	| '+'
	| '-'
	;

zero_or_one :
	integer
	;

integer :
	maybe_sign_token kIntegerToken maybe_whitespace
	;

non_negative_integer :
	integer
	;

positive_integer :
	integer
	;

number :
	integer
	| maybe_sign_token kRealToken maybe_whitespace
	;

non_negative_number :
	number
	;

percentage :
	maybe_sign_token kPercentageToken maybe_whitespace
	;

positive_percentage :
	percentage
	;

alpha :
	number
	;

length :
	number
	| absolute_or_relative_length
	;

non_negative_absolute_or_relative_length :
	absolute_or_relative_length
	;

absolute_or_relative_length :
	maybe_sign_token kFontSizesAkaEmToken maybe_whitespace
	| maybe_sign_token kRootElementFontSizesAkaRemToken maybe_whitespace
	| maybe_sign_token kViewportWidthPercentsAkaVwToken maybe_whitespace
	| maybe_sign_token kViewportHeightPercentsAkaVhToken maybe_whitespace
	| maybe_sign_token kPixelsToken maybe_whitespace
	;

positive_length :
	length
	;

ratio :
	positive_integer '/' maybe_whitespace positive_integer
	;

resolution :
	maybe_sign_token kDotsPerInchToken maybe_whitespace
	| maybe_sign_token kDotsPerCentimeterToken maybe_whitespace
	;

angle :
	maybe_sign_token kDegreesToken maybe_whitespace
	| maybe_sign_token kGradiansToken maybe_whitespace
	| maybe_sign_token kRadiansToken maybe_whitespace
	| maybe_sign_token kTurnsToken maybe_whitespace
	;

time :
	number
	| time_with_units_required
	;

time_with_units_required :
	maybe_sign_token kSecondsToken maybe_whitespace
	| maybe_sign_token kMillisecondsToken maybe_whitespace
	;

colon :
	':' maybe_whitespace
	;

comma :
	',' maybe_whitespace
	;

common_values :
	common_values_without_errors
	| errors
	;

common_values_without_errors :
	kInheritToken maybe_whitespace
	| kInitialToken maybe_whitespace
	;

color :
	kHexToken maybe_whitespace
	| kRGBFunctionToken maybe_whitespace integer comma integer comma integer ')' maybe_whitespace
	| kRGBAFunctionToken maybe_whitespace integer comma integer comma integer comma alpha ')' maybe_whitespace
	| kHslFunctionToken maybe_whitespace integer comma positive_percentage comma positive_percentage ')'
	| kHslaFunctionToken maybe_whitespace integer comma positive_percentage comma positive_percentage comma alpha ')'
	| kAquaToken maybe_whitespace
	| kBlackToken maybe_whitespace
	| kBlueToken maybe_whitespace
	| kFuchsiaToken maybe_whitespace
	| kGrayToken maybe_whitespace
	| kGreenToken maybe_whitespace
	| kLimeToken maybe_whitespace
	| kMaroonToken maybe_whitespace
	| kNavyToken maybe_whitespace
	| kOliveToken maybe_whitespace
	| kPurpleToken maybe_whitespace
	| kRedToken maybe_whitespace
	| kSilverToken maybe_whitespace
	| kTealToken maybe_whitespace
	| kTransparentToken maybe_whitespace
	| kWhiteToken maybe_whitespace
	| kYellowToken maybe_whitespace
	;

url :
	kUriToken ')' maybe_whitespace
	;

length_percent_property_value :
	length
	| percentage
	;

positive_length_percent_property_value :
	positive_length
	| positive_percentage
	;

background_property_element :
	color
	| background_image_property_list_element
	;

maybe_background_size_property_value :
	/*empty*/
	| '/' maybe_whitespace background_size_property_value_without_common_values
	;

background_position_and_size_shorthand_property_value :
	validated_position_property maybe_background_size_property_value
	;

background_repeat_shorthand_property_value :
	background_repeat_property_value_without_common_values
	;

background_position_and_repeat_combination :
	background_position_and_size_shorthand_property_value
	| background_repeat_shorthand_property_value
	| background_position_and_size_shorthand_property_value background_repeat_shorthand_property_value
	| background_repeat_shorthand_property_value background_position_and_size_shorthand_property_value
	;

final_background_layer_without_position_and_repeat :
	/*empty*/
	| final_background_layer background_property_element
	;

final_background_layer :
	/*empty*/
	| final_background_layer_without_position_and_repeat background_position_and_repeat_combination
	| final_background_layer background_property_element
	;

background_property_value :
	final_background_layer
	| common_values
	;

background_color_property_value :
	color
	| common_values
	;

color_stop :
	color
	| color length_percent_property_value
	;

comma_separated_color_stop_list :
	color_stop comma color_stop
	| comma_separated_color_stop_list comma color_stop
	;

side :
	kBottomToken maybe_whitespace
	| kLeftToken maybe_whitespace
	| kRightToken maybe_whitespace
	| kTopToken maybe_whitespace
	;

side_or_corner :
	side
	| side side
	;

linear_gradient_params :
	comma_separated_color_stop_list
	| angle comma comma_separated_color_stop_list
	| kToToken kWhitespaceToken maybe_whitespace side_or_corner comma comma_separated_color_stop_list
	;

size_keyword :
	kClosestSideToken maybe_whitespace
	| kFarthestSideToken maybe_whitespace
	| kClosestCornerToken maybe_whitespace
	| kFarthestCornerToken maybe_whitespace
	;

circle_with_size_keyword :
	kCircleToken maybe_whitespace size_keyword
	| size_keyword kCircleToken maybe_whitespace
	;

circle_with_positive_length :
	positive_length
	| positive_length kCircleToken maybe_whitespace
	| kCircleToken maybe_whitespace positive_length
	;

maybe_ellipse_with_size_keyword :
	kEllipseToken maybe_whitespace size_keyword
	| size_keyword kEllipseToken maybe_whitespace
	| size_keyword
	;

ellipse_with_2_positive_length_percents :
	positive_length_percent_property_value positive_length_percent_property_value
	| positive_length_percent_property_value positive_length_percent_property_value kEllipseToken maybe_whitespace
	| kEllipseToken maybe_whitespace positive_length_percent_property_value positive_length_percent_property_value
	;

at_position :
	kAtToken maybe_whitespace validated_position_property
	;

maybe_at_position :
	/*empty*/
	| at_position
	;

radial_gradient_params :
	circle_with_positive_length maybe_at_position comma comma_separated_color_stop_list
	| ellipse_with_2_positive_length_percents maybe_at_position comma comma_separated_color_stop_list
	| circle_with_size_keyword maybe_at_position comma comma_separated_color_stop_list
	| maybe_ellipse_with_size_keyword maybe_at_position comma comma_separated_color_stop_list
	| kCircleToken maybe_whitespace maybe_at_position comma comma_separated_color_stop_list
	| kEllipseToken maybe_whitespace maybe_at_position comma comma_separated_color_stop_list
	| at_position comma comma_separated_color_stop_list
	| comma_separated_color_stop_list
	;

background_image_property_list_element :
	url
	| kLinearGradientFunctionToken maybe_whitespace linear_gradient_params ')'
	| kRadialGradientFunctionToken maybe_whitespace radial_gradient_params ')'
	| kNoneToken maybe_whitespace
	;

comma_separated_background_image_list :
	background_image_property_list_element
	| comma_separated_background_image_list comma background_image_property_list_element
	;

background_image_property_value :
	comma_separated_background_image_list
	| common_values
	;

position_list_element :
	kBottomToken maybe_whitespace
	| kCenterToken maybe_whitespace
	| kLeftToken maybe_whitespace
	| kRightToken maybe_whitespace
	| kTopToken maybe_whitespace
	| length_percent_property_value
	;

position_list :
	position_list_element
	| position_list position_list_element
	;

validated_position_property :
	position_list
	;

background_position_property_value :
	validated_position_property
	| common_values
	;

background_repeat_element :
	kNoRepeatToken maybe_whitespace
	| kRepeatToken maybe_whitespace
	;

background_repeat_property_value_without_common_values :
	background_repeat_element
	| background_repeat_element background_repeat_element
	| kRepeatXToken maybe_whitespace
	| kRepeatYToken maybe_whitespace
	;

background_repeat_property_value :
	background_repeat_property_value_without_common_values
	| common_values
	;

background_size_property_list_element :
	length
	| positive_percentage
	| auto
	;

background_size_property_list :
	background_size_property_list_element
	| background_size_property_list_element background_size_property_list_element
	;

background_size_property_value_without_common_values :
	background_size_property_list
	| kContainToken maybe_whitespace
	| kCoverToken maybe_whitespace
	;

background_size_property_value :
	background_size_property_value_without_common_values
	| common_values
	;

border_color_property_list :
	/*empty*/
	| border_color_property_list color
	;

border_color_property_value :
	border_color_property_list
	| common_values
	;

line_style :
	kNoneToken maybe_whitespace
	| kHiddenToken maybe_whitespace
	| kSolidToken maybe_whitespace
	;

line_style_with_common_values :
	line_style
	| common_values
	;

border_style_property_list :
	/*empty*/
	| border_style_property_list line_style
	;

border_style_property_value :
	border_style_property_list
	| common_values
	;

border_width_element :
	positive_length
	;

border_width_element_with_common_values :
	border_width_element
	| common_values
	;

border_width_property_list :
	/*empty*/
	| border_width_property_list border_width_element
	;

border_width_property_value :
	border_width_property_list
	| common_values
	;

border_or_outline_property_element :
	color
	| line_style
	| positive_length
	;

border_or_outline_property_list :
	/*empty*/
	| border_or_outline_property_list border_or_outline_property_element
	;

border_or_outline_property_value :
	border_or_outline_property_list
	| common_values
	;

border_radius_element :
	positive_length_percent_property_value
	;

border_radius_element_with_common_values :
	border_radius_element
	| common_values
	;

border_radius_property_list :
	/*empty*/
	| border_radius_property_list border_radius_element
	;

border_radius_property_value :
	border_radius_property_list
	| common_values
	;

box_shadow_property_element :
	length
	| color
	| kInsetToken maybe_whitespace
	;

box_shadow_list :
	/*empty*/
	| box_shadow_list box_shadow_property_element
	;

validated_box_shadow_list :
	box_shadow_list
	;

comma_separated_box_shadow_list :
	validated_box_shadow_list
	| comma_separated_box_shadow_list comma validated_box_shadow_list
	;

box_shadow_property_value :
	comma_separated_box_shadow_list
	| kNoneToken maybe_whitespace
	| common_values
	;

color_property_value :
	color
	| common_values
	;

content_property_value :
	kNoneToken maybe_whitespace
	| kNormalToken maybe_whitespace
	| url
	| kStringToken maybe_whitespace
	| common_values
	;

display_property_value :
	kBlockToken maybe_whitespace
	| kFlexToken maybe_whitespace
	| kInlineToken maybe_whitespace
	| kInlineBlockToken maybe_whitespace
	| kInlineFlexToken maybe_whitespace
	| kNoneToken maybe_whitespace
	| common_values
	;

font_face_url_src :
	url kFormatFunctionToken maybe_whitespace kStringToken maybe_whitespace ')' maybe_whitespace
	| url
	;

font_face_local_src :
	kLocalFunctionToken maybe_whitespace font_family_specific_name ')' maybe_whitespace
	;

font_face_src_list_element :
	font_face_url_src
	| font_face_local_src
	;

comma_separated_font_face_src_list :
	font_face_src_list_element
	| comma_separated_font_face_src_list comma font_face_src_list_element
	;

font_face_src_property_value :
	comma_separated_font_face_src_list
	| common_values
	;

font_family_name_identifier_list :
	identifier_token maybe_whitespace
	| font_family_name_identifier_list identifier_token maybe_whitespace
	;

font_family_string_name :
	kStringToken maybe_whitespace
	;

font_family_specific_name :
	font_family_name_identifier_list
	| font_family_string_name
	;

font_family_specific_name_no_single_identifier :
	font_family_name_identifier_list identifier_token maybe_whitespace
	| font_family_string_name
	;

font_family_name :
	identifier_token maybe_whitespace
	| font_family_specific_name_no_single_identifier
	;

comma_separated_font_family_name_list :
	font_family_name
	| comma_separated_font_family_name_list comma font_family_name
	;

font_family_property_value :
	identifier_token maybe_whitespace
	| font_family_specific_name_no_single_identifier
	| comma_separated_font_family_name_list comma font_family_name
	| errors
	;

font_size_property_value :
	positive_length_percent_property_value
	| common_values
	;

font_style_exclusive_property_value :
	kItalicToken maybe_whitespace
	| kObliqueToken maybe_whitespace
	;

font_style_property_value :
	font_style_exclusive_property_value
	| kNormalToken maybe_whitespace
	| common_values
	;

font_weight_exclusive_property_value :
	kBoldToken maybe_whitespace
	| positive_integer
	;

font_weight_property_value :
	font_weight_exclusive_property_value
	| kNormalToken maybe_whitespace
	| common_values
	;

optional_font_element :
	font_style_exclusive_property_value
	| kNormalToken maybe_whitespace
	| error maybe_whitespace
	;

optional_font_value_list :
	/*empty*/
	| non_empty_optional_font_value_list
	;

non_empty_optional_font_value_list :
	optional_font_value_list optional_font_element
	;

font_property_value :
	optional_font_value_list positive_length_percent_property_value comma_separated_font_family_name_list
	| optional_font_value_list font_weight_exclusive_property_value positive_length_percent_property_value comma_separated_font_family_name_list
	| common_values_without_errors
	;

height_property_value :
	positive_length_percent_property_value
	| auto
	| common_values
	;

intersection_observer_root_margin_property_list :
	length_percent_property_value
	| length_percent_property_value length_percent_property_value
	| length_percent_property_value length_percent_property_value length_percent_property_value
	| length_percent_property_value length_percent_property_value length_percent_property_value length_percent_property_value
	;

intersection_observer_root_margin_property_value :
	intersection_observer_root_margin_property_list
	;

min_height_property_value :
	kAutoToken maybe_whitespace
	| positive_length_percent_property_value
	| common_values
	;

max_height_property_value :
	kNoneToken maybe_whitespace
	| positive_length_percent_property_value
	| common_values
	;

line_height_property_value :
	kNormalToken maybe_whitespace
	| non_negative_number
	| absolute_or_relative_length
	| positive_percentage
	| common_values
	;

margin_width :
	length_percent_property_value
	| auto
	;

margin_side_property_value :
	margin_width
	| common_values
	;

margin_property_value :
	margin_width
	| margin_width margin_width
	| margin_width margin_width margin_width
	| margin_width margin_width margin_width margin_width
	| common_values
	;

offset_property_value :
	length_percent_property_value
	| auto
	| common_values
	;

opacity_property_value :
	alpha
	| common_values
	;

overflow_property_value :
	kAutoToken maybe_whitespace
	| kHiddenToken maybe_whitespace
	| kScrollToken maybe_whitespace
	| kVisibleToken maybe_whitespace
	| common_values
	;

overflow_wrap_property_value :
	kBreakWordToken maybe_whitespace
	| kNormalToken maybe_whitespace
	| common_values
	;

padding_side_property_value :
	positive_length_percent_property_value
	| common_values
	;

padding_property_value :
	positive_length_percent_property_value
	| positive_length_percent_property_value positive_length_percent_property_value
	| positive_length_percent_property_value positive_length_percent_property_value positive_length_percent_property_value
	| positive_length_percent_property_value positive_length_percent_property_value positive_length_percent_property_value positive_length_percent_property_value
	| common_values
	;

pointer_events_property_value :
	kAutoToken maybe_whitespace
	| kNoneToken maybe_whitespace
	| common_values
	;

position_property_value :
	kAbsoluteToken maybe_whitespace
	| kFixedToken maybe_whitespace
	| kRelativeToken maybe_whitespace
	| kStaticToken maybe_whitespace
	| common_values
	;

scale_function_parameters :
	number
	| number comma number
	;

text_align_property_value :
	kEndToken maybe_whitespace
	| kLeftToken maybe_whitespace
	| kCenterToken maybe_whitespace
	| kRightToken maybe_whitespace
	| kStartToken maybe_whitespace
	| common_values
	;

text_decoration_line :
	kNoneToken maybe_whitespace
	| kLineThroughToken maybe_whitespace
	;

text_decoration_line_property_value :
	text_decoration_line
	| common_values
	;

text_decoration_property_element :
	text_decoration_line
	| color
	;

text_decoration_property_list :
	/*empty*/
	| text_decoration_property_list text_decoration_property_element
	;

text_decoration_property_value :
	text_decoration_property_list
	| common_values
	;

text_indent_property_value :
	length
	| common_values
	;

text_overflow_property_value :
	kClipToken maybe_whitespace
	| kEllipsisToken maybe_whitespace
	| common_values
	;

text_shadow_property_element :
	length
	| color
	;

text_shadow_list :
	/*empty*/
	| text_shadow_list text_shadow_property_element
	;

validated_text_shadow_list :
	text_shadow_list
	;

comma_separated_text_shadow_list :
	validated_text_shadow_list
	| comma_separated_text_shadow_list comma validated_text_shadow_list
	;

text_shadow_property_value :
	comma_separated_text_shadow_list
	| kNoneToken maybe_whitespace
	| common_values
	;

text_transform_property_value :
	kNoneToken maybe_whitespace
	| kUppercaseToken maybe_whitespace
	| common_values
	;

transform_function :
	kMatrixFunctionToken maybe_whitespace number comma number comma number comma number comma number comma number ')' maybe_whitespace
	| kRotateFunctionToken maybe_whitespace angle ')' maybe_whitespace
	| kScaleFunctionToken maybe_whitespace scale_function_parameters ')' maybe_whitespace
	| kScaleXFunctionToken maybe_whitespace number ')' maybe_whitespace
	| kScaleYFunctionToken maybe_whitespace number ')' maybe_whitespace
	| kTranslateFunctionToken maybe_whitespace length_percent_property_value ')' maybe_whitespace
	| kTranslateFunctionToken maybe_whitespace length_percent_property_value comma length_percent_property_value ')' maybe_whitespace
	| kTranslateXFunctionToken maybe_whitespace length_percent_property_value ')' maybe_whitespace
	| kTranslateYFunctionToken maybe_whitespace length_percent_property_value ')' maybe_whitespace
	| kTranslateZFunctionToken maybe_whitespace length ')' maybe_whitespace
	| kCobaltUiNavFocusTransformFunctionToken maybe_whitespace ')' maybe_whitespace
	| kCobaltUiNavFocusTransformFunctionToken maybe_whitespace number ')' maybe_whitespace
	| kCobaltUiNavFocusTransformFunctionToken maybe_whitespace number comma number ')' maybe_whitespace
	| kCobaltUiNavSpotlightTransformFunctionToken maybe_whitespace ')' maybe_whitespace
	;

transform_list :
	/*empty*/
	| transform_list transform_function
	;

transform_property_value :
	kNoneToken maybe_whitespace
	| transform_list
	| transform_list errors
	| common_values_without_errors
	;

validated_two_position_list_elements :
	position_list_element position_list_element
	;

transform_origin_property_value :
	position_list_element
	| validated_two_position_list_elements
	| validated_two_position_list_elements length
	| common_values
	;

vertical_align_property_value :
	kBottomToken maybe_whitespace
	| kTopToken maybe_whitespace
	| kMiddleToken maybe_whitespace
	| kBaselineToken maybe_whitespace
	| common_values
	;

visibility_property_value :
	kCollapseToken maybe_whitespace
	| kHiddenToken maybe_whitespace
	| kVisibleToken maybe_whitespace
	| common_values
	;

comma_separated_time_list :
	time
	| comma_separated_time_list comma time
	;

time_list_property_value :
	comma_separated_time_list
	| common_values
	;

maybe_steps_start_or_end_parameter :
	/*empty*/
	| comma kStartToken maybe_whitespace
	| comma kEndToken maybe_whitespace
	;

single_timing_function :
	kCubicBezierFunctionToken maybe_whitespace number comma number comma number comma number ')'
	| kStepsFunctionToken maybe_whitespace kIntegerToken maybe_whitespace maybe_steps_start_or_end_parameter ')'
	| kEaseInOutToken
	| kEaseInToken
	| kEaseOutToken
	| kEaseToken
	| kLinearToken
	| kStepEndToken
	| kStepStartToken
	;

comma_separated_single_timing_function_list :
	single_timing_function
	| comma_separated_single_timing_function_list comma single_timing_function
	;

timing_function_list_property_value :
	comma_separated_single_timing_function_list
	| common_values
	;

animation_delay_property_value :
	time_list_property_value
	;

animation_direction_list_element :
	kNormalToken maybe_whitespace
	| kReverseToken maybe_whitespace
	| kAlternateToken maybe_whitespace
	| kAlternateReverseToken maybe_whitespace
	;

comma_separated_animation_direction_list :
	animation_direction_list_element
	| comma_separated_animation_direction_list comma animation_direction_list_element
	;

animation_direction_property_value :
	comma_separated_animation_direction_list
	| common_values_without_errors
	| errors
	;

animation_duration_property_value :
	time_list_property_value
	;

animation_fill_mode_list_element :
	kNoneToken maybe_whitespace
	| kForwardsToken maybe_whitespace
	| kBackwardsToken maybe_whitespace
	| kBothToken maybe_whitespace
	;

comma_separated_animation_fill_mode_list :
	animation_fill_mode_list_element
	| comma_separated_animation_fill_mode_list comma animation_fill_mode_list_element
	;

animation_fill_mode_property_value :
	comma_separated_animation_fill_mode_list
	| common_values_without_errors
	| errors
	;

animation_iteration_count_list_element :
	kInfiniteToken maybe_whitespace
	| non_negative_number
	;

comma_separated_animation_iteration_count_list :
	animation_iteration_count_list_element
	| comma_separated_animation_iteration_count_list comma animation_iteration_count_list_element
	;

animation_iteration_count_property_value :
	comma_separated_animation_iteration_count_list
	| common_values_without_errors
	| errors
	;

animation_name_list_element :
	kNoneToken maybe_whitespace
	| kIdentifierToken maybe_whitespace
	;

comma_separated_animation_name_list :
	animation_name_list_element
	| comma_separated_animation_name_list comma animation_name_list_element
	;

animation_name_property_value :
	comma_separated_animation_name_list
	| common_values_without_errors
	| errors
	;

animation_timing_function_property_value :
	timing_function_list_property_value
	;

single_animation_element :
	kIdentifierToken maybe_whitespace
	| animation_direction_list_element
	| animation_fill_mode_list_element
	| animation_iteration_count_list_element
	| time_with_units_required
	| single_timing_function maybe_whitespace
	| error maybe_whitespace
	;

single_animation :
	/*empty*/
	| single_non_empty_animation
	;

single_non_empty_animation :
	single_animation single_animation_element
	;

comma_separated_animation_list :
	single_non_empty_animation
	| comma_separated_animation_list comma single_non_empty_animation
	;

animation_property_value :
	comma_separated_animation_list
	| common_values_without_errors
	;

transition_delay_property_value :
	time_list_property_value
	;

transition_duration_property_value :
	time_list_property_value
	;

transition_timing_function_property_value :
	timing_function_list_property_value
	;

comma_separated_animatable_property_name_list :
	animatable_property_token maybe_whitespace
	| comma_separated_animatable_property_name_list comma animatable_property_token maybe_whitespace
	| errors
	;

transition_property_property_value :
	kNoneToken maybe_whitespace
	| comma_separated_animatable_property_name_list
	| common_values_without_errors
	;

single_transition_element :
	animatable_property_token maybe_whitespace
	| kNoneToken maybe_whitespace
	| time
	| single_timing_function maybe_whitespace
	| error maybe_whitespace
	;

single_transition :
	/*empty*/
	| single_non_empty_transition
	;

single_non_empty_transition :
	single_transition single_transition_element
	;

comma_separated_transition_list :
	single_non_empty_transition
	| comma_separated_transition_list comma single_non_empty_transition
	;

transition_property_value :
	comma_separated_transition_list
	| common_values_without_errors
	;

unicode_range_property_value :
	comma_separated_unicode_range_list
	| common_values
	;

comma_separated_unicode_range_list :
	kUnicodeRangeToken maybe_whitespace
	| comma_separated_unicode_range_list comma kUnicodeRangeToken maybe_whitespace
	;

white_space_property_value :
	kNormalToken maybe_whitespace
	| kNowrapToken maybe_whitespace
	| kPreToken maybe_whitespace
	| kPreLineToken maybe_whitespace
	| kPreWrapToken maybe_whitespace
	| common_values
	;

width_property_value :
	positive_length_percent_property_value
	| auto
	| common_values
	;

min_width_property_value :
	kAutoToken maybe_whitespace
	| positive_length_percent_property_value
	| common_values
	;

max_width_property_value :
	kNoneToken maybe_whitespace
	| positive_length_percent_property_value
	| common_values
	;

maybe_important :
	/*empty*/
	| kImportantToken maybe_whitespace
	;

z_index_property_value :
	integer
	| auto
	| common_values
	;

flex_start_end_center_property_value :
	kFlexStartToken maybe_whitespace
	| kFlexEndToken maybe_whitespace
	| kCenterToken maybe_whitespace
	;

space_between_around_property_value :
	kSpaceBetweenToken maybe_whitespace
	| kSpaceAroundToken maybe_whitespace
	;

baseline_stretch_property_value :
	kBaselineToken maybe_whitespace
	| kStretchToken maybe_whitespace
	;

justify_content_property_value :
	flex_start_end_center_property_value
	| space_between_around_property_value
	| common_values
	;

align_content_property_value :
	flex_start_end_center_property_value
	| space_between_around_property_value
	| kStretchToken maybe_whitespace
	| common_values
	;

align_items_property_value :
	flex_start_end_center_property_value
	| baseline_stretch_property_value
	| common_values
	;

align_self_property_value :
	kAutoToken maybe_whitespace
	| flex_start_end_center_property_value
	| baseline_stretch_property_value
	| common_values
	;

flex_basis_keywords :
	kContentToken maybe_whitespace
	| kAutoToken maybe_whitespace
	;

flex_basis_property_value :
	flex_basis_keywords
	| positive_length_percent_property_value
	| common_values
	;

flex_basis_element :
	flex_basis_keywords
	| non_negative_absolute_or_relative_length
	| positive_percentage
	;

flex_single_flex_basis_element :
	kContentToken maybe_whitespace
	| non_negative_absolute_or_relative_length
	| positive_percentage
	;

flex_single_property_value :
	flex_single_flex_basis_element
	| non_negative_number
	| kAutoToken maybe_whitespace
	| kNoneToken maybe_whitespace
	;

flex_two_property_values :
	non_negative_number non_negative_number
	| non_negative_number flex_basis_element
	| flex_basis_element non_negative_number
	;

flex_three_property_values :
	non_negative_number non_negative_number flex_basis_element
	| flex_basis_element non_negative_number non_negative_number
	| non_negative_number non_negative_number non_negative_number
	;

flex_property_value :
	flex_single_property_value
	| flex_two_property_values
	| flex_three_property_values
	| common_values
	;

flex_factor_property_value :
	non_negative_number
	| common_values
	;

order_property_value :
	integer
	| common_values
	;

flex_direction :
	kRowToken maybe_whitespace
	| kRowReverseToken maybe_whitespace
	| kColumnToken maybe_whitespace
	| kColumnReverseToken maybe_whitespace
	;

flex_direction_property_value :
	flex_direction
	| common_values
	;

flex_flow_property_element :
	flex_direction
	| flex_wrap
	;

flex_flow_property_list :
	/*empty*/
	| flex_flow_property_list flex_flow_property_element
	;

flex_flow_property_value :
	flex_flow_property_list
	| common_values
	;

flex_wrap :
	kNowrapToken maybe_whitespace
	| kWrapToken maybe_whitespace
	| kWrapReverseToken maybe_whitespace
	;

flex_wrap_property_value :
	flex_wrap
	| common_values
	;

animatable_property_token :
	kAllToken
	| kBackgroundColorToken
	| kBorderBottomColorToken
	| kBorderLeftColorToken
	| kBorderRightColorToken
	| kBorderTopColorToken
	| kColorToken
	| kOpacityToken
	| kOutlineColorToken
	| kOutlineWidthToken
	| kTransformToken
	;

maybe_declaration :
	/*empty*/
	| kAlignContentToken maybe_whitespace colon align_content_property_value maybe_important
	| kAlignItemsToken maybe_whitespace colon align_items_property_value maybe_important
	| kAlignSelfToken maybe_whitespace colon align_self_property_value maybe_important
	| kAnimationDelayToken maybe_whitespace colon animation_delay_property_value maybe_important
	| kAnimationDirectionToken maybe_whitespace colon animation_direction_property_value maybe_important
	| kAnimationDurationToken maybe_whitespace colon animation_duration_property_value maybe_important
	| kAnimationFillModeToken maybe_whitespace colon animation_fill_mode_property_value maybe_important
	| kAnimationIterationCountToken maybe_whitespace colon animation_iteration_count_property_value maybe_important
	| kAnimationNameToken maybe_whitespace colon animation_name_property_value maybe_important
	| kAnimationTimingFunctionToken maybe_whitespace colon animation_timing_function_property_value maybe_important
	| kAnimationToken maybe_whitespace colon animation_property_value maybe_important
	| kBackgroundToken maybe_whitespace colon background_property_value maybe_important
	| kBackgroundColorToken maybe_whitespace colon background_color_property_value maybe_important
	| kBackgroundImageToken maybe_whitespace colon background_image_property_value maybe_important
	| kBackgroundPositionToken maybe_whitespace colon background_position_property_value maybe_important
	| kBackgroundRepeatToken maybe_whitespace colon background_repeat_property_value maybe_important
	| kBackgroundSizeToken maybe_whitespace colon background_size_property_value maybe_important
	| kBorderToken maybe_whitespace colon border_or_outline_property_value maybe_important
	| kBorderBottomLeftRadiusToken maybe_whitespace colon border_radius_element_with_common_values maybe_important
	| kBorderBottomRightRadiusToken maybe_whitespace colon border_radius_element_with_common_values maybe_important
	| kBorderBottomToken maybe_whitespace colon border_or_outline_property_value maybe_important
	| kBorderBottomColorToken maybe_whitespace colon color_property_value maybe_important
	| kBorderBottomStyleToken maybe_whitespace colon line_style_with_common_values maybe_important
	| kBorderBottomWidthToken maybe_whitespace colon border_width_element_with_common_values maybe_important
	| kBorderColorToken maybe_whitespace colon border_color_property_value maybe_important
	| kBorderLeftToken maybe_whitespace colon border_or_outline_property_value maybe_important
	| kBorderLeftColorToken maybe_whitespace colon color_property_value maybe_important
	| kBorderLeftStyleToken maybe_whitespace colon line_style_with_common_values maybe_important
	| kBorderLeftWidthToken maybe_whitespace colon border_width_element_with_common_values maybe_important
	| kBorderRadiusToken maybe_whitespace colon border_radius_property_value maybe_important
	| kBorderRightToken maybe_whitespace colon border_or_outline_property_value maybe_important
	| kBorderRightColorToken maybe_whitespace colon color_property_value maybe_important
	| kBorderRightStyleToken maybe_whitespace colon line_style_with_common_values maybe_important
	| kBorderRightWidthToken maybe_whitespace colon border_width_element_with_common_values maybe_important
	| kBorderStyleToken maybe_whitespace colon border_style_property_value maybe_important
	| kBorderTopToken maybe_whitespace colon border_or_outline_property_value maybe_important
	| kBorderTopColorToken maybe_whitespace colon color_property_value maybe_important
	| kBorderTopLeftRadiusToken maybe_whitespace colon border_radius_element_with_common_values maybe_important
	| kBorderTopRightRadiusToken maybe_whitespace colon border_radius_element_with_common_values maybe_important
	| kBorderTopStyleToken maybe_whitespace colon line_style_with_common_values maybe_important
	| kBorderTopWidthToken maybe_whitespace colon border_width_element_with_common_values maybe_important
	| kBorderWidthToken maybe_whitespace colon border_width_property_value maybe_important
	| kBottomToken maybe_whitespace colon offset_property_value maybe_important
	| kBoxShadowToken maybe_whitespace colon box_shadow_property_value maybe_important
	| kColorToken maybe_whitespace colon color_property_value maybe_important
	| kContentToken maybe_whitespace colon content_property_value maybe_important
	| kDisplayToken maybe_whitespace colon display_property_value maybe_important
	| kFilterToken maybe_whitespace colon filter_property_value maybe_important
	| kFlexToken maybe_whitespace colon flex_property_value maybe_important
	| kFlexBasisToken maybe_whitespace colon flex_basis_property_value maybe_important
	| kFlexDirectionToken maybe_whitespace colon flex_direction_property_value maybe_important
	| kFlexFlowToken maybe_whitespace colon flex_flow_property_value maybe_important
	| kFlexGrowToken maybe_whitespace colon flex_factor_property_value maybe_important
	| kFlexShrinkToken maybe_whitespace colon flex_factor_property_value maybe_important
	| kFlexWrapToken maybe_whitespace colon flex_wrap_property_value maybe_important
	| kFontToken maybe_whitespace colon font_property_value maybe_important
	| kFontFamilyToken maybe_whitespace colon font_family_property_value maybe_important
	| kFontSizeToken maybe_whitespace colon font_size_property_value maybe_important
	| kFontStyleToken maybe_whitespace colon font_style_property_value maybe_important
	| kFontWeightToken maybe_whitespace colon font_weight_property_value maybe_important
	| kHeightToken maybe_whitespace colon height_property_value maybe_important
	| kIntersectionObserverRootMarginToken maybe_whitespace colon intersection_observer_root_margin_property_value maybe_important
	| kJustifyContentToken maybe_whitespace colon justify_content_property_value maybe_important
	| kLeftToken maybe_whitespace colon offset_property_value maybe_important
	| kLineHeightToken maybe_whitespace colon line_height_property_value maybe_important
	| kMarginBottomToken maybe_whitespace colon margin_side_property_value maybe_important
	| kMarginLeftToken maybe_whitespace colon margin_side_property_value maybe_important
	| kMarginRightToken maybe_whitespace colon margin_side_property_value maybe_important
	| kMarginTopToken maybe_whitespace colon margin_side_property_value maybe_important
	| kMarginToken maybe_whitespace colon margin_property_value maybe_important
	| kMaxHeightToken maybe_whitespace colon max_height_property_value maybe_important
	| kMaxWidthToken maybe_whitespace colon max_width_property_value maybe_important
	| kMinHeightToken maybe_whitespace colon min_height_property_value maybe_important
	| kMinWidthToken maybe_whitespace colon min_width_property_value maybe_important
	| kOpacityToken maybe_whitespace colon opacity_property_value maybe_important
	| kOrderToken maybe_whitespace colon order_property_value maybe_important
	| kOutlineToken maybe_whitespace colon border_or_outline_property_value maybe_important
	| kOutlineColorToken maybe_whitespace colon color_property_value maybe_important
	| kOutlineStyleToken maybe_whitespace colon line_style_with_common_values maybe_important
	| kOutlineWidthToken maybe_whitespace colon border_width_element_with_common_values maybe_important
	| kOverflowToken maybe_whitespace colon overflow_property_value maybe_important
	| kOverflowWrapToken maybe_whitespace colon overflow_wrap_property_value maybe_important
	| kPaddingBottomToken maybe_whitespace colon padding_side_property_value maybe_important
	| kPaddingLeftToken maybe_whitespace colon padding_side_property_value maybe_important
	| kPaddingRightToken maybe_whitespace colon padding_side_property_value maybe_important
	| kPaddingTopToken maybe_whitespace colon padding_side_property_value maybe_important
	| kPaddingToken maybe_whitespace colon padding_property_value maybe_important
	| kPointerEventsToken maybe_whitespace colon pointer_events_property_value maybe_important
	| kPositionToken maybe_whitespace colon position_property_value maybe_important
	| kRightToken maybe_whitespace colon offset_property_value maybe_important
	| kSrcToken maybe_whitespace colon font_face_src_property_value maybe_important
	| kTextAlignToken maybe_whitespace colon text_align_property_value maybe_important
	| kTextIndentToken maybe_whitespace colon text_indent_property_value maybe_important
	| kTextDecorationToken maybe_whitespace colon text_decoration_property_value maybe_important
	| kTextDecorationColorToken maybe_whitespace colon color_property_value maybe_important
	| kTextDecorationLineToken maybe_whitespace colon text_decoration_line_property_value maybe_important
	| kTextOverflowToken maybe_whitespace colon text_overflow_property_value maybe_important
	| kTextShadowToken maybe_whitespace colon text_shadow_property_value maybe_important
	| kTextTransformToken maybe_whitespace colon text_transform_property_value maybe_important
	| kTopToken maybe_whitespace colon offset_property_value maybe_important
	| kTransformToken maybe_whitespace colon transform_property_value maybe_important
	| kTransformOriginToken maybe_whitespace colon transform_origin_property_value maybe_important
	| kTransitionToken maybe_whitespace colon transition_property_value maybe_important
	| kTransitionDelayToken maybe_whitespace colon transition_delay_property_value maybe_important
	| kTransitionDurationToken maybe_whitespace colon transition_duration_property_value maybe_important
	| kTransitionPropertyToken maybe_whitespace colon transition_property_property_value maybe_important
	| kTransitionTimingFunctionToken maybe_whitespace colon transition_timing_function_property_value maybe_important
	| kUnicodeRangePropertyToken maybe_whitespace colon unicode_range_property_value maybe_important
	| kVerticalAlignToken maybe_whitespace colon vertical_align_property_value maybe_important
	| kVisibilityToken maybe_whitespace colon visibility_property_value maybe_important
	| kWhiteSpacePropertyToken maybe_whitespace colon white_space_property_value maybe_important
	| kWidthToken maybe_whitespace colon width_property_value maybe_important
	| kWordWrapToken maybe_whitespace colon overflow_wrap_property_value maybe_important
	| kZIndexToken maybe_whitespace colon z_index_property_value maybe_important
	| kIdentifierToken maybe_whitespace colon errors
	| errors
	;

semicolon :
	';' maybe_whitespace
	;

style_declaration_list :
	maybe_declaration
	| style_declaration_list semicolon maybe_declaration
	;

font_face_declaration_list :
	maybe_declaration
	| font_face_declaration_list semicolon maybe_declaration
	;

style_declaration_block :
	'{' maybe_whitespace style_declaration_list '}' maybe_whitespace
	;

rule_list_block :
	'{' maybe_whitespace rule_list '}' maybe_whitespace
	| semicolon
	;

style_rule :
	selector_list style_declaration_block
	;

qualified_rule :
	style_rule
	| error style_declaration_block
	;

invalid_rule :
	kInvalidAtBlockToken maybe_whitespace
	| kOtherBrowserAtBlockToken maybe_whitespace
	;

rule :
	kSgmlCommentDelimiterToken maybe_whitespace
	| qualified_rule
	| at_font_face_rule
	| at_media_rule
	| at_keyframes_rule
	| invalid_rule
	;

rule_list :
	/*empty*/
	| rule_list rule
	;

style_sheet :
	rule_list
	;

entry_point :
	kStyleSheetEntryPointToken maybe_whitespace style_sheet
	| kMediaListEntryPointToken maybe_whitespace media_list
	| kMediaQueryEntryPointToken maybe_whitespace media_query
	| kRuleEntryPointToken maybe_whitespace rule
	| kStyleDeclarationListEntryPointToken maybe_whitespace style_declaration_list
	| kFontFaceDeclarationListEntryPointToken maybe_whitespace font_face_declaration_list
	| kPropertyValueEntryPointToken maybe_whitespace maybe_declaration
	| kPropertyIntoDeclarationDataEntryPointToken maybe_whitespace maybe_declaration
	;

filter_property_value :
	kNoneToken maybe_whitespace
	| filter_function_list
	| common_values
	;

filter_function_list :
	filter_function
	;

filter_function :
	cobalt_mtm_filter_function
	;

cobalt_mtm_filter_function :
	cobalt_mtm_function_name maybe_whitespace cobalt_map_to_mesh_spec comma angle angle comma cobalt_mtm_transform_function maybe_cobalt_mtm_stereo_mode ')' maybe_whitespace
	| cobalt_mtm_function_name maybe_whitespace kRectangularToken comma kNoneToken comma kNoneToken maybe_cobalt_mtm_stereo_mode ')' maybe_whitespace
	;

cobalt_map_to_mesh_spec :
	kEquirectangularToken
	| url cobalt_mtm_resolution_matched_mesh_list
	;

cobalt_mtm_function_name :
	kCobaltMtmFunctionToken
	| kMapToMeshFunctionToken
	;

cobalt_mtm_resolution_matched_mesh_list :
	/*empty*/
	| cobalt_mtm_resolution_matched_mesh_list cobalt_mtm_resolution_matched_mesh
	;

cobalt_mtm_resolution_matched_mesh :
	non_negative_integer non_negative_integer url
	;

cobalt_mtm_transform_function :
	kMatrix3dFunctionToken maybe_whitespace number_matrix ')' maybe_whitespace
	;

number_matrix :
	number
	| number_matrix comma number
	;

maybe_cobalt_mtm_stereo_mode :
	/*empty*/
	| comma cobalt_mtm_stereo_mode
	;

cobalt_mtm_stereo_mode :
	kMonoscopicToken maybe_whitespace
	| kStereoscopicLeftRightToken maybe_whitespace
	| kStereoscopicTopBottomToken maybe_whitespace
	;

//Lexer

kEndOfFileToken : 'kEndOfFileToken' ;
kWhitespaceToken : 'kWhitespaceToken' ;
kFontFaceToken : 'kFontFaceToken' ;
kMediaToken : 'kMediaToken' ;
kLandscapeMediaFeatureKeywordValueToken : 'kLandscapeMediaFeatureKeywordValueToken' ;
kPortraitMediaFeatureKeywordValueToken : 'kPortraitMediaFeatureKeywordValueToken' ;
kInterlaceMediaFeatureKeywordValueToken : 'kInterlaceMediaFeatureKeywordValueToken' ;
kProgressiveMediaFeatureKeywordValueToken : 'kProgressiveMediaFeatureKeywordValueToken' ;
kMediaMinimumToken : 'kMediaMinimumToken' ;
kMediaMaximumToken : 'kMediaMaximumToken' ;
kLengthMediaFeatureTypeToken : 'kLengthMediaFeatureTypeToken' ;
kOrientationMediaFeatureTypeToken : 'kOrientationMediaFeatureTypeToken' ;
kRatioMediaFeatureTypeToken : 'kRatioMediaFeatureTypeToken' ;
kNonNegativeIntegerMediaFeatureTypeToken : 'kNonNegativeIntegerMediaFeatureTypeToken' ;
kResolutionMediaFeatureTypeToken : 'kResolutionMediaFeatureTypeToken' ;
kScanMediaFeatureTypeToken : 'kScanMediaFeatureTypeToken' ;
kZeroOrOneMediaFeatureTypeToken : 'kZeroOrOneMediaFeatureTypeToken' ;
kMediaAndToken : 'kMediaAndToken' ;
kIdentifierToken : 'kIdentifierToken' ;
kAllMediaTypeToken : 'kAllMediaTypeToken' ;
kTVMediaTypeToken : 'kTVMediaTypeToken' ;
kScreenMediaTypeToken : 'kScreenMediaTypeToken' ;
kMediaNotToken : 'kMediaNotToken' ;
kMediaOnlyToken : 'kMediaOnlyToken' ;
kKeyframesToken : 'kKeyframesToken' ;
kFromToken : 'kFromToken' ;
kToToken : 'kToToken' ;
kPercentageToken : 'kPercentageToken' ;
kAllToken : 'kAllToken' ;
kAlignContentToken : 'kAlignContentToken' ;
kAlignItemsToken : 'kAlignItemsToken' ;
kAlignSelfToken : 'kAlignSelfToken' ;
kAnimationDelayToken : 'kAnimationDelayToken' ;
kAnimationDirectionToken : 'kAnimationDirectionToken' ;
kAnimationDurationToken : 'kAnimationDurationToken' ;
kAnimationFillModeToken : 'kAnimationFillModeToken' ;
kAnimationIterationCountToken : 'kAnimationIterationCountToken' ;
kAnimationNameToken : 'kAnimationNameToken' ;
kAnimationTimingFunctionToken : 'kAnimationTimingFunctionToken' ;
kAnimationToken : 'kAnimationToken' ;
kBackgroundColorToken : 'kBackgroundColorToken' ;
kBackgroundImageToken : 'kBackgroundImageToken' ;
kBackgroundPositionToken : 'kBackgroundPositionToken' ;
kBackgroundRepeatToken : 'kBackgroundRepeatToken' ;
kBackgroundSizeToken : 'kBackgroundSizeToken' ;
kBackgroundToken : 'kBackgroundToken' ;
kBorderToken : 'kBorderToken' ;
kBorderBottomLeftRadiusToken : 'kBorderBottomLeftRadiusToken' ;
kBorderBottomRightRadiusToken : 'kBorderBottomRightRadiusToken' ;
kBorderBottomToken : 'kBorderBottomToken' ;
kBorderBottomColorToken : 'kBorderBottomColorToken' ;
kBorderBottomStyleToken : 'kBorderBottomStyleToken' ;
kBorderBottomWidthToken : 'kBorderBottomWidthToken' ;
kBorderLeftToken : 'kBorderLeftToken' ;
kBorderColorToken : 'kBorderColorToken' ;
kBorderLeftColorToken : 'kBorderLeftColorToken' ;
kBorderLeftStyleToken : 'kBorderLeftStyleToken' ;
kBorderLeftWidthToken : 'kBorderLeftWidthToken' ;
kBorderRadiusToken : 'kBorderRadiusToken' ;
kBorderRightToken : 'kBorderRightToken' ;
kBorderRightColorToken : 'kBorderRightColorToken' ;
kBorderRightStyleToken : 'kBorderRightStyleToken' ;
kBorderRightWidthToken : 'kBorderRightWidthToken' ;
kBorderStyleToken : 'kBorderStyleToken' ;
kBorderTopToken : 'kBorderTopToken' ;
kBorderTopColorToken : 'kBorderTopColorToken' ;
kBorderTopLeftRadiusToken : 'kBorderTopLeftRadiusToken' ;
kBorderTopRightRadiusToken : 'kBorderTopRightRadiusToken' ;
kBorderTopStyleToken : 'kBorderTopStyleToken' ;
kBorderTopWidthToken : 'kBorderTopWidthToken' ;
kBorderWidthToken : 'kBorderWidthToken' ;
kBottomToken : 'kBottomToken' ;
kBoxShadowToken : 'kBoxShadowToken' ;
kColorToken : 'kColorToken' ;
kContentToken : 'kContentToken' ;
kDisplayToken : 'kDisplayToken' ;
kFilterToken : 'kFilterToken' ;
kFlexToken : 'kFlexToken' ;
kFlexBasisToken : 'kFlexBasisToken' ;
kFlexDirectionToken : 'kFlexDirectionToken' ;
kFlexFlowToken : 'kFlexFlowToken' ;
kFlexGrowToken : 'kFlexGrowToken' ;
kFlexShrinkToken : 'kFlexShrinkToken' ;
kFlexWrapToken : 'kFlexWrapToken' ;
kFontToken : 'kFontToken' ;
kFontFamilyToken : 'kFontFamilyToken' ;
kFontSizeToken : 'kFontSizeToken' ;
kFontStyleToken : 'kFontStyleToken' ;
kFontWeightToken : 'kFontWeightToken' ;
kHeightToken : 'kHeightToken' ;
kIntersectionObserverRootMarginToken : 'kIntersectionObserverRootMarginToken' ;
kJustifyContentToken : 'kJustifyContentToken' ;
kLeftToken : 'kLeftToken' ;
kLineHeightToken : 'kLineHeightToken' ;
kMarginBottomToken : 'kMarginBottomToken' ;
kMarginLeftToken : 'kMarginLeftToken' ;
kMarginRightToken : 'kMarginRightToken' ;
kMarginToken : 'kMarginToken' ;
kMarginTopToken : 'kMarginTopToken' ;
kMaxHeightToken : 'kMaxHeightToken' ;
kMaxWidthToken : 'kMaxWidthToken' ;
kMinHeightToken : 'kMinHeightToken' ;
kMinWidthToken : 'kMinWidthToken' ;
kOpacityToken : 'kOpacityToken' ;
kOrderToken : 'kOrderToken' ;
kOutlineToken : 'kOutlineToken' ;
kOutlineColorToken : 'kOutlineColorToken' ;
kOutlineStyleToken : 'kOutlineStyleToken' ;
kOutlineWidthToken : 'kOutlineWidthToken' ;
kOverflowToken : 'kOverflowToken' ;
kOverflowWrapToken : 'kOverflowWrapToken' ;
kPaddingBottomToken : 'kPaddingBottomToken' ;
kPaddingLeftToken : 'kPaddingLeftToken' ;
kPaddingRightToken : 'kPaddingRightToken' ;
kPaddingToken : 'kPaddingToken' ;
kPaddingTopToken : 'kPaddingTopToken' ;
kPointerEventsToken : 'kPointerEventsToken' ;
kPositionToken : 'kPositionToken' ;
kRightToken : 'kRightToken' ;
kSrcToken : 'kSrcToken' ;
kTextAlignToken : 'kTextAlignToken' ;
kTextDecorationToken : 'kTextDecorationToken' ;
kTextDecorationColorToken : 'kTextDecorationColorToken' ;
kTextDecorationLineToken : 'kTextDecorationLineToken' ;
kTextIndentToken : 'kTextIndentToken' ;
kTextOverflowToken : 'kTextOverflowToken' ;
kTextShadowToken : 'kTextShadowToken' ;
kTextTransformToken : 'kTextTransformToken' ;
kTopToken : 'kTopToken' ;
kTransformToken : 'kTransformToken' ;
kTransformOriginToken : 'kTransformOriginToken' ;
kTransitionDelayToken : 'kTransitionDelayToken' ;
kTransitionDurationToken : 'kTransitionDurationToken' ;
kTransitionPropertyToken : 'kTransitionPropertyToken' ;
kTransitionTimingFunctionToken : 'kTransitionTimingFunctionToken' ;
kTransitionToken : 'kTransitionToken' ;
kUnicodeRangePropertyToken : 'kUnicodeRangePropertyToken' ;
kVerticalAlignToken : 'kVerticalAlignToken' ;
kVisibilityToken : 'kVisibilityToken' ;
kWhiteSpacePropertyToken : 'kWhiteSpacePropertyToken' ;
kWidthToken : 'kWidthToken' ;
kWordWrapToken : 'kWordWrapToken' ;
kZIndexToken : 'kZIndexToken' ;
kAbsoluteToken : 'kAbsoluteToken' ;
kAlternateToken : 'kAlternateToken' ;
kAlternateReverseToken : 'kAlternateReverseToken' ;
kAquaToken : 'kAquaToken' ;
kAtToken : 'kAtToken' ;
kAutoToken : 'kAutoToken' ;
kBackwardsToken : 'kBackwardsToken' ;
kBaselineToken : 'kBaselineToken' ;
kBlackToken : 'kBlackToken' ;
kBlockToken : 'kBlockToken' ;
kBlueToken : 'kBlueToken' ;
kBoldToken : 'kBoldToken' ;
kBothToken : 'kBothToken' ;
kBreakWordToken : 'kBreakWordToken' ;
kCenterToken : 'kCenterToken' ;
kCircleToken : 'kCircleToken' ;
kClipToken : 'kClipToken' ;
kClosestCornerToken : 'kClosestCornerToken' ;
kClosestSideToken : 'kClosestSideToken' ;
kCollapseToken : 'kCollapseToken' ;
kColumnToken : 'kColumnToken' ;
kColumnReverseToken : 'kColumnReverseToken' ;
kContainToken : 'kContainToken' ;
kCoverToken : 'kCoverToken' ;
kCursiveToken : 'kCursiveToken' ;
kEaseInOutToken : 'kEaseInOutToken' ;
kEaseInToken : 'kEaseInToken' ;
kEaseOutToken : 'kEaseOutToken' ;
kEaseToken : 'kEaseToken' ;
kEllipseToken : 'kEllipseToken' ;
kEllipsisToken : 'kEllipsisToken' ;
kEndToken : 'kEndToken' ;
kEquirectangularToken : 'kEquirectangularToken' ;
kFantasyToken : 'kFantasyToken' ;
kFarthestCornerToken : 'kFarthestCornerToken' ;
kFarthestSideToken : 'kFarthestSideToken' ;
kFixedToken : 'kFixedToken' ;
kFlexEndToken : 'kFlexEndToken' ;
kFlexStartToken : 'kFlexStartToken' ;
kForwardsToken : 'kForwardsToken' ;
kFuchsiaToken : 'kFuchsiaToken' ;
kGrayToken : 'kGrayToken' ;
kGreenToken : 'kGreenToken' ;
kHiddenToken : 'kHiddenToken' ;
kInfiniteToken : 'kInfiniteToken' ;
kInheritToken : 'kInheritToken' ;
kInitialToken : 'kInitialToken' ;
kInlineBlockToken : 'kInlineBlockToken' ;
kInlineToken : 'kInlineToken' ;
kInlineFlexToken : 'kInlineFlexToken' ;
kInsetToken : 'kInsetToken' ;
kItalicToken : 'kItalicToken' ;
kLimeToken : 'kLimeToken' ;
kLinearToken : 'kLinearToken' ;
kLineThroughToken : 'kLineThroughToken' ;
kMaroonToken : 'kMaroonToken' ;
kMiddleToken : 'kMiddleToken' ;
kMonoscopicToken : 'kMonoscopicToken' ;
kMonospaceToken : 'kMonospaceToken' ;
kNavyToken : 'kNavyToken' ;
kNoneToken : 'kNoneToken' ;
kNoRepeatToken : 'kNoRepeatToken' ;
kNormalToken : 'kNormalToken' ;
kNowrapToken : 'kNowrapToken' ;
kObliqueToken : 'kObliqueToken' ;
kOliveToken : 'kOliveToken' ;
kPreToken : 'kPreToken' ;
kPreLineToken : 'kPreLineToken' ;
kPreWrapToken : 'kPreWrapToken' ;
kPurpleToken : 'kPurpleToken' ;
kRedToken : 'kRedToken' ;
kRepeatToken : 'kRepeatToken' ;
kRepeatXToken : 'kRepeatXToken' ;
kRepeatYToken : 'kRepeatYToken' ;
kRelativeToken : 'kRelativeToken' ;
kReverseToken : 'kReverseToken' ;
kRowToken : 'kRowToken' ;
kRowReverseToken : 'kRowReverseToken' ;
kSansSerifToken : 'kSansSerifToken' ;
kScrollToken : 'kScrollToken' ;
kSerifToken : 'kSerifToken' ;
kSilverToken : 'kSilverToken' ;
kSolidToken : 'kSolidToken' ;
kSpaceAroundToken : 'kSpaceAroundToken' ;
kSpaceBetweenToken : 'kSpaceBetweenToken' ;
kStartToken : 'kStartToken' ;
kStaticToken : 'kStaticToken' ;
kStepEndToken : 'kStepEndToken' ;
kStepStartToken : 'kStepStartToken' ;
kStereoscopicLeftRightToken : 'kStereoscopicLeftRightToken' ;
kStereoscopicTopBottomToken : 'kStereoscopicTopBottomToken' ;
kStretchToken : 'kStretchToken' ;
kTealToken : 'kTealToken' ;
kTransparentToken : 'kTransparentToken' ;
kUppercaseToken : 'kUppercaseToken' ;
kVisibleToken : 'kVisibleToken' ;
kWhiteToken : 'kWhiteToken' ;
kWrapToken : 'kWrapToken' ;
kWrapReverseToken : 'kWrapReverseToken' ;
kYellowToken : 'kYellowToken' ;
kActiveToken : 'kActiveToken' ;
kEmptyToken : 'kEmptyToken' ;
kFocusToken : 'kFocusToken' ;
kHoverToken : 'kHoverToken' ;
kAfterToken : 'kAfterToken' ;
kBeforeToken : 'kBeforeToken' ;
kIncludesToken : 'kIncludesToken' ;
kDashMatchToken : 'kDashMatchToken' ;
kBeginsWithToken : 'kBeginsWithToken' ;
kEndsWithToken : 'kEndsWithToken' ;
kContainsToken : 'kContainsToken' ;
kStringToken : 'kStringToken' ;
kIdSelectorToken : 'kIdSelectorToken' ;
kHexToken : 'kHexToken' ;
kNotFunctionToken : 'kNotFunctionToken' ;
kIntegerToken : 'kIntegerToken' ;
kRealToken : 'kRealToken' ;
kFontSizesAkaEmToken : 'kFontSizesAkaEmToken' ;
kRootElementFontSizesAkaRemToken : 'kRootElementFontSizesAkaRemToken' ;
kViewportWidthPercentsAkaVwToken : 'kViewportWidthPercentsAkaVwToken' ;
kViewportHeightPercentsAkaVhToken : 'kViewportHeightPercentsAkaVhToken' ;
kPixelsToken : 'kPixelsToken' ;
kDotsPerInchToken : 'kDotsPerInchToken' ;
kDotsPerCentimeterToken : 'kDotsPerCentimeterToken' ;
kDegreesToken : 'kDegreesToken' ;
kGradiansToken : 'kGradiansToken' ;
kRadiansToken : 'kRadiansToken' ;
kTurnsToken : 'kTurnsToken' ;
kSecondsToken : 'kSecondsToken' ;
kMillisecondsToken : 'kMillisecondsToken' ;
kRGBFunctionToken : 'kRGBFunctionToken' ;
kRGBAFunctionToken : 'kRGBAFunctionToken' ;
kHslFunctionToken : 'kHslFunctionToken' ;
kHslaFunctionToken : 'kHslaFunctionToken' ;
kUriToken : 'kUriToken' ;
kLinearGradientFunctionToken : 'kLinearGradientFunctionToken' ;
kRadialGradientFunctionToken : 'kRadialGradientFunctionToken' ;
kFormatFunctionToken : 'kFormatFunctionToken' ;
kLocalFunctionToken : 'kLocalFunctionToken' ;
kMatrixFunctionToken : 'kMatrixFunctionToken' ;
kRotateFunctionToken : 'kRotateFunctionToken' ;
kScaleFunctionToken : 'kScaleFunctionToken' ;
kScaleXFunctionToken : 'kScaleXFunctionToken' ;
kScaleYFunctionToken : 'kScaleYFunctionToken' ;
kTranslateFunctionToken : 'kTranslateFunctionToken' ;
kTranslateXFunctionToken : 'kTranslateXFunctionToken' ;
kTranslateYFunctionToken : 'kTranslateYFunctionToken' ;
kTranslateZFunctionToken : 'kTranslateZFunctionToken' ;
kCobaltUiNavFocusTransformFunctionToken : 'kCobaltUiNavFocusTransformFunctionToken' ;
kCobaltUiNavSpotlightTransformFunctionToken : 'kCobaltUiNavSpotlightTransformFunctionToken' ;
kCubicBezierFunctionToken : 'kCubicBezierFunctionToken' ;
kStepsFunctionToken : 'kStepsFunctionToken' ;
kUnicodeRangeToken : 'kUnicodeRangeToken' ;
kImportantToken : 'kImportantToken' ;
kInvalidAtBlockToken : 'kInvalidAtBlockToken' ;
kOtherBrowserAtBlockToken : 'kOtherBrowserAtBlockToken' ;
kSgmlCommentDelimiterToken : 'kSgmlCommentDelimiterToken' ;
kStyleSheetEntryPointToken : 'kStyleSheetEntryPointToken' ;
kMediaListEntryPointToken : 'kMediaListEntryPointToken' ;
kMediaQueryEntryPointToken : 'kMediaQueryEntryPointToken' ;
kRuleEntryPointToken : 'kRuleEntryPointToken' ;
kStyleDeclarationListEntryPointToken : 'kStyleDeclarationListEntryPointToken' ;
kFontFaceDeclarationListEntryPointToken : 'kFontFaceDeclarationListEntryPointToken' ;
kPropertyValueEntryPointToken : 'kPropertyValueEntryPointToken' ;
kPropertyIntoDeclarationDataEntryPointToken : 'kPropertyIntoDeclarationDataEntryPointToken' ;
kRectangularToken : 'kRectangularToken' ;
kCobaltMtmFunctionToken : 'kCobaltMtmFunctionToken' ;
kMapToMeshFunctionToken : 'kMapToMeshFunctionToken' ;
kMatrix3dFunctionToken : 'kMatrix3dFunctionToken' ;

}
