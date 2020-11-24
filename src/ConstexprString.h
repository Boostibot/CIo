#ifndef CONSTEXPRSTRING_H
#define CONSTEXPRSTRING_H

#include "Common.h"
#include "AuxiliaryTypes.h"

namespace cio
{
    template<typename CharT, Size capacity>
    struct ConstexprString
    {
            using CharType = CharT;
            using ThisType = ConstexprString<CharType, capacity>;
            using SizeType = cio::Size;

        private:
            static constexpr Size TotalSize = capacity + 1;
            static constexpr CharT NullTermination = '\0';

            Size CurrentSize = 0;
            CharType String[TotalSize] = {};

        public:
            constexpr ConstexprString() : CurrentSize(0), String()
            {
                FinishStringAt(0);
            }

            constexpr ConstexprString(const ThisType REF) = default;
            constexpr ConstexprString(ThisType RVALUE_REF) = default;
            constexpr ThisType REF operator=(const ThisType REF) = default;
            constexpr ThisType REF operator=(ThisType RVALUE_REF) = default;

            constexpr ConstexprString(const CharType * string) : CurrentSize(0), String()
            {
                Assign(string, this->Capacity());
            }

            static constexpr auto Capacity() noexcept {return capacity;}
            constexpr auto Size() const noexcept {return CurrentSize;}

            constexpr const CharType REF operator[](const SizeType at) const
            {
                return String[at];
            }
            constexpr CharType REF operator[](const SizeType at)
            {
                return String[at];
            }
            constexpr ThisType REF operator+=(const ThisType other)
            {
                Assign(other.String, other.Size(), CurrentSize);
                return PTR_VAL(this);
            }

            constexpr ThisType REF operator+=(const CharT singleChar)
            {
                Assign(ADDRESS(singleChar), 1, CurrentSize);
                return PTR_VAL(this);
            }

            constexpr bool operator==(const ThisType string) const
            {
                if(this->CurrentSize != string.CurrentSize)
                    return false;

                for(SizeType i = 0; i < this->CurrentSize; i++)
                {
                    if(this->String[i] != string[i])
                        return false;
                }

                return true;
            }
            constexpr bool operator!=(const ThisType string) const
            {
                if(this->CurrentSize != string.CurrentSize)
                    return true;

                for(SizeType i = 0; i < this->CurrentSize; i++)
                {
                    if(this->String[i] != string[i])
                        return true;
                }

                return false;
            }

            constexpr operator std::basic_string_view<CharT>() const noexcept
            {
                return std::basic_string_view<CharT>(ADDRESS(String[0]), CurrentSize);
            }

        private:
            //Does not check if it is in bounds
            constexpr void FinishStringAt(SizeType atPos)
            {
                CurrentSize = atPos;
                String[atPos] = NullTermination;
            }

            constexpr bool Assign(const CharT PTR data, SizeType size, SizeType atPos = 0)
            {
                struct Helper{ static constexpr bool WereAllCharsCopied(SizeType index, SizeType size) {return index >= size;} };

                SizeType indexInThis = atPos;
                SizeType indexInData = 0;

                for(;
                    indexInThis < capacity && indexInData < size;
                    indexInThis++, indexInData++)
                {
                    String[indexInThis] = data[indexInData];

                    if(String[indexInThis] == ThisType::NullTermination)
                    {
                        FinishStringAt(indexInThis);
                        return true;
                    }
                }


                FinishStringAt(indexInThis);

                return Helper::WereAllCharsCopied(indexInData, size);
            }
    };

    template<typename CharTo, typename CharFrom, Size strSize>
    constexpr auto PromoteStringCharsTo(const ConstexprString<CharFrom, strSize> str) noexcept
    {
        static_assert (sizeof (CharFrom) <= sizeof (CharTo), "CharFrom needs to be smaller char size than CharTo");
        ConstexprString<CharTo, strSize> returnStr;

        for(Size i = 0; i < str.Size(); i++)
            returnStr += static_cast<CharTo>(str[i]);

        return returnStr;
    }
}

#endif // CONSTEXPRSTRING_H
