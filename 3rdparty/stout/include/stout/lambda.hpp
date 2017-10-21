// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __STOUT_LAMBDA_HPP__
#define __STOUT_LAMBDA_HPP__

#include <algorithm>
#include <functional>
#include <vector>

#include <stout/hashmap.hpp>
#include <stout/result_of.hpp>

namespace lambda {

using std::bind;
using std::cref;
using std::function;
using std::ref;

using namespace std::placeholders;


template <
  template <typename...> class Iterable,
  typename F,
  typename U,
  typename V = typename result_of<F(U)>::type,
  typename... Us>
Iterable<V> map(F&& f, const Iterable<U, Us...>& input)
{
  Iterable<V> output;
  std::transform(
      input.begin(),
      input.end(),
      std::inserter(output, output.begin()),
      std::forward<F>(f));
  return output;
}


template <
  template <typename...> class OutputIterable,
  template <typename...> class InputIterable,
  typename F,
  typename U,
  typename V = typename result_of<F(U)>::type,
  typename... Us>
OutputIterable<V> map(F&& f, const InputIterable<U, Us...>& input)
{
  OutputIterable<V> output;
  std::transform(
      input.begin(),
      input.end(),
      std::inserter(output, output.begin()),
      std::forward<F>(f));
  return output;
}


template <
  template <typename...> class Iterable,
  typename F,
  typename U,
  typename V = typename result_of<F(U)>::type,
  typename = typename std::enable_if<
    !std::is_same<U, V>::value>::type,
  typename... Us>
Iterable<V> map(F&& f, Iterable<U, Us...>&& input)
{
  Iterable<V> output;
  std::transform(
      std::make_move_iterator(input.begin()),
      std::make_move_iterator(input.end()),
      std::inserter(output, output.begin()),
      std::forward<F>(f));
  return output;
}


template <
  template <typename...> class Iterable,
  typename F,
  typename U,
  typename = typename std::enable_if<
    std::is_same<U, typename result_of<F(U)>::type>::value>::type,
  typename... Us>
Iterable<U, Us...>&& map(F&& f, Iterable<U, Us...>&& iterable)
{
  std::transform(
      std::make_move_iterator(iterable.begin()),
      std::make_move_iterator(iterable.end()),
      iterable.begin(),
      std::forward<F>(f));
  return std::move(iterable);
}


template <
  template <typename...> class OutputIterable,
  template <typename...> class InputIterable,
  typename F,
  typename U,
  typename V = typename result_of<F(U)>::type,
  typename... Us>
OutputIterable<V> map(F&& f, InputIterable<U, Us...>&& input)
{
  OutputIterable<V> output;
  std::transform(
      std::make_move_iterator(input.begin()),
      std::make_move_iterator(input.end()),
      std::inserter(output, output.begin()),
      std::forward<F>(f));
  return output;
}


template <
  template <typename...> class OutputIterable,
  typename F,
  typename U,
  typename V = typename result_of<F(U)>::type>
OutputIterable<V> map(F&& f, std::initializer_list<U> input)
{
  OutputIterable<V> output;
  std::transform(
      input.begin(),
      input.end(),
      std::inserter(output, output.begin()),
      std::forward<F>(f));
  return output;
}


template <
  typename F,
  typename U,
  typename V = typename result_of<F(U)>::type>
std::vector<V> map(F&& f, std::initializer_list<U> input)
{
  std::vector<V> output;
  std::transform(
      input.begin(),
      input.end(),
      std::inserter(output, output.begin()),
      std::forward<F>(f));
  return output;
}


template <
  template <typename...> class OutputIterable,
  template <typename...> class InputIterable1,
  template <typename...> class InputIterable2,
  typename U1,
  typename U2,
  typename... U1s,
  typename... U2s>
OutputIterable<std::pair<U1, U2>> zipto(
    const InputIterable1<U1, U1s...>& input1,
    const InputIterable2<U2, U2s...>& input2)
{
  OutputIterable<std::pair<U1, U2>> output;

  auto iterator1 = input1.begin();
  auto iterator2 = input2.begin();

  auto inserter = std::inserter(output, output.begin());

  // We zip only as many elements as we can.
  while (iterator1 != input1.end() && iterator2 != input2.end()) {
    inserter = std::make_pair(*iterator1, *iterator2);
    iterator1++;
    iterator2++;
  }

  return output;
}


// NOTE: by default we zip into a `hashmap`. See the `zip()` overload
// for zipping into another iterable as `std::pair`.
template <
  template <typename...> class InputIterable1,
  template <typename...> class InputIterable2,
  typename U1,
  typename U2,
  typename... U1s,
  typename... U2s>
hashmap<U1, U2> zip(
    const InputIterable1<U1, U1s...>& input1,
    const InputIterable2<U2, U2s...>& input2)
{
  // TODO(benh): Use the overload of `zip()`, something like:
  //   std::vector<std::pair<U1, U2>> vector = zip<std::vector>(input1, input2);
  //   return hashmap<U1, U2>(
  //     std::make_move_iterator(vector.begin()),
  //     std::make_move_iterator(vector.end()));

  hashmap<U1, U2> output;

  auto iterator1 = input1.begin();
  auto iterator2 = input2.begin();

  // We zip only as many elements as we can.
  while (iterator1 != input1.end() && iterator2 != input2.end()) {
    output.put(*iterator1, *iterator2);
    iterator1++;
    iterator2++;
  }

  return output;
}

} // namespace lambda {

#endif // __STOUT_LAMBDA_HPP__
