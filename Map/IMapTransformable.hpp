#ifndef IMAPTRANSFORMABLE_HPP
# define IMAPTRANSFORMABLE_HPP

class IMapTransformable
{
public:
		virtual ~IMapTransformable(void) = default;

		virtual void swapDepth(void) = 0;
		virtual void registerDepth(void) = 0;
		virtual bool nextStep(void) = 0;
		virtual bool previousStep(void) = 0;

};

#endif
